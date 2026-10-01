#include "Commands/World/Editor/CommandListEditor.hpp"

#include "Commands/World/Editor/joaat_hash_db.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/ControllerInputs.hpp"
#include "Menu/Click.hpp"
#include "Menu/GUI.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/Label.hpp"
#include "Util/Joaat.hpp"

#include "Commands/Widgets/CommandSliderFloat.hpp"

#include "Rendering/Grid.hpp"
#include "Rendering/GridItem.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <cmath>
#include <string>
#include <string_view>

namespace Stand
{
    namespace
    {
        static int s_cam = 0;
        static const char* s_preview_model = nullptr;
        static int         s_preview_handle = 0;
        static std::atomic<uint32_t> s_virtualFocusCounter{0};
        static std::atomic<int> s_spawnerLeaveCount{0};
        static float s_preview_heading = 0.0f;
        static std::string simplified(std::string_view s)
        {
            std::string out;
            out.reserve(s.size());
            for (char c : s)
                if (c != '_' && c != ' ' && c != '-')
                    out += (char)std::tolower((unsigned char)c);
            return out;
        }

        class EditorPreviewTick;

        class EditorActivePresence : public CommandPhysical
        {
            EditorPreviewTick* m_tick;
        public:
            EditorActivePresence(CommandList* parent, EditorPreviewTick* tick)
                : CommandPhysical(COMMAND_ACTION, parent, NOLABEL, CMDNAMES_0(), NOLABEL)
                , m_tick(tick)
            {}
            void fillVirtualItems(std::vector<std::unique_ptr<Rendering::GridItem>>& items, Rendering::Grid* grid) override;
        };

        class EditorSectionHeader : public CommandPhysical
        {
        public:
            EditorSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent,
                    Label(label, Label::TagLiteral{}), {}, NOLABEL,
                    CMDFLAG_SECTION_HEADER) {}
        };

        class EditorSpawnedObject;

        class EditorSpawnedObjectDelete : public CommandPhysical
        {
            EditorSpawnedObject* m_spawned;
        public:
            EditorSpawnedObjectDelete(CommandList* parent, EditorSpawnedObject* spawned)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Delete"), CMDNAMES_0(), NOLABEL)
                , m_spawned(spawned) {}

            void onClick(Click& click) override;
        };

        class EditorEntityPositionAxis : public CommandSliderFloat
        {
            int m_handle;
            int m_axis;
        public:
            EditorEntityPositionAxis(CommandList* parent, int handle, int axis, const char* name, int minV, int maxV, int defV)
                : CommandSliderFloat(parent, Label(name, Label::TagLiteral{}), CMDNAMES_0(), NOLABEL, minV, maxV, defV, 10)
                , m_handle(handle), m_axis(axis)
            { precision = 1; }

            void onChange(Click&, int) override
            {
                const float newVal = getFloatValue();
                const int h = m_handle;
                const int ax = m_axis;
                FiberPool::queueJob([h, ax, newVal] {
                    if (!h) return;
                    Vector3 pos = ENTITY::GET_ENTITY_COORDS(h, FALSE);
                    if (ax == 0) pos.x = newVal;
                    else if (ax == 1) pos.y = newVal;
                    else pos.z = newVal;
                    ENTITY::SET_ENTITY_COORDS_NO_OFFSET(h, pos.x, pos.y, pos.z, FALSE, FALSE, FALSE);
                });
            }
        };

        class EditorEntityPosition : public CommandList
        {
        public:
            EditorEntityPosition(CommandList* parent, int handle, const Vector3& initialPos)
                : CommandList(parent, LIT("Position"))
            {
                const int ix = (int)(initialPos.x * 10.0f);
                const int iy = (int)(initialPos.y * 10.0f);
                const int iz = (int)(initialPos.z * 10.0f);
                createChild<EditorEntityPositionAxis>(handle, 0, "X", -45000, 45000, ix);
                createChild<EditorEntityPositionAxis>(handle, 1, "Y", -45000, 45000, iy);
                createChild<EditorEntityPositionAxis>(handle, 2, "Z", -2000, 20000, iz);
            }
        };

        class EditorSpawnedObject : public CommandList
        {
        public:
            int handle;

            EditorSpawnedObject(CommandList* parent, const char* name, int h)
                : CommandList(parent, Label(name, Label::TagLiteral{}))
                , handle(h)
            {
                createChild<EditorSpawnedObjectDelete>(this);
                const Vector3 pos = ENTITY::GET_ENTITY_COORDS(h, FALSE);
                createChild<EditorEntityPosition>(h, pos);
            }

            ~EditorSpawnedObject() override
            {
                if (handle) {
                    int h = handle;
                    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(h, FALSE, TRUE);
                    ENTITY::DELETE_ENTITY(&h);
                }
            }
        };

        void EditorSpawnedObjectDelete::onClick(Click& click)
        {
            const int h = m_spawned->handle;
            m_spawned->handle = 0;
            EditorSpawnedObject* spawned = m_spawned;

            {
                auto& spawnedGrid = Rendering::GridStandCommandList::GetOrCreate(spawned);
                while (Rendering::MenuNavigation::IsDescendantActive(&spawnedGrid))
                    Rendering::MenuNavigation::Pop();
            }

            FiberPool::queueJob([h, spawned] {
                if (h) {
                    int hh = h;
                    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(hh, FALSE, TRUE);
                    ENTITY::DELETE_ENTITY(&hh);
                }
                auto* editor = spawned->parent->as<CommandListEditor>();
                auto& ch = editor->children;
                for (auto it = ch.begin(); it != ch.end(); ++it) {
                    if (it->get() == spawned) {
                        ch.erase(it);
                        break;
                    }
                }
                editor->spawnedVersion.fetch_add(1, std::memory_order_relaxed);
            });
        }

        class EditorFreecam : public CommandToggle
        {
        public:
            explicit EditorFreecam(CommandList* parent)
                : CommandToggle(parent, LIT("Freecam"), CMDNAMES("editorfreecam"), NOLABEL, true)
            { CommandTickDispatch::AddCommand(this); }

            ~EditorFreecam() override
            {
                CommandTickDispatch::RemoveCommand(this);
                if (s_cam) destroyCam();
            }

            void onEnable(Click& click) override
            {
                click.ensureScriptThread([] {
                    Vector3 pp = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(-1), TRUE);
                    s_cam = CAMERA::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", TRUE);
                    CAMERA::SET_CAM_COORD(s_cam, pp.x, pp.y, pp.z + 5.0f);
                    CAMERA::SET_CAM_ROT(s_cam, -30.0f, 0.0f, 0.0f, 2);
                    CAMERA::SET_CAM_FOV(s_cam, 60.0f);
                    CAMERA::SET_CAM_ACTIVE(s_cam, TRUE);
                    CAMERA::RENDER_SCRIPT_CAMS(TRUE, FALSE, 0, TRUE, FALSE, 0);
                });
            }

            void onDisable(Click& click) override
            {
                click.ensureScriptThread([] { EditorFreecam::destroyCam(); });
            }

            void onTick() override
            {
                if (!m_on || !s_cam)
                    return;

                Vector3 pos = CAMERA::GET_CAM_COORD(s_cam);
                Vector3 rot = CAMERA::GET_CAM_ROT(s_cam, 2);

                const float mlr = PAD::GET_DISABLED_CONTROL_NORMAL(0, (int)ControllerInputs::INPUT_LOOK_LR);
                const float mud = PAD::GET_DISABLED_CONTROL_NORMAL(0, (int)ControllerInputs::INPUT_LOOK_UD);
                rot.z -= mlr * 5.0f;
                rot.x = std::clamp(rot.x + mud * 5.0f, -89.0f, 89.0f);

                const float speed = PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_SPRINT) ? 2.0f : 0.5f;
                const float yaw   = rot.z * 0.01745329f;

                if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_UP_ONLY)) {
                    pos.x += speed * sinf(-yaw);
                    pos.y += speed * cosf(-yaw);
                }
                if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_DOWN_ONLY)) {
                    pos.x -= speed * sinf(-yaw);
                    pos.y -= speed * cosf(-yaw);
                }
                if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_LEFT_ONLY)) {
                    pos.x += speed * sinf(-yaw + 1.5707963f);
                    pos.y += speed * cosf(-yaw + 1.5707963f);
                }
                if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_RIGHT_ONLY)) {
                    pos.x -= speed * sinf(-yaw + 1.5707963f);
                    pos.y -= speed * cosf(-yaw + 1.5707963f);
                }
                if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_JUMP))
                    pos.z += speed;
                if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_DUCK))
                    pos.z -= speed;

                CAMERA::SET_CAM_COORD(s_cam, pos.x, pos.y, pos.z);
                CAMERA::SET_CAM_ROT(s_cam, rot.x, rot.y, rot.z, 2);
            }

        private:
            static void destroyCam()
            {
                if (s_cam) {
                    CAMERA::SET_CAM_ACTIVE(s_cam, FALSE);
                    CAMERA::RENDER_SCRIPT_CAMS(FALSE, TRUE, 0, FALSE, FALSE, 0);
                    CAMERA::DESTROY_CAM(s_cam, FALSE);
                    s_cam = 0;
                }
            }
        };

        class EditorTeleport : public CommandPhysical
        {
        public:
            explicit EditorTeleport(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent,
                    LIT("Teleport To Where I'm Looking"),
                    CMDNAMES("editorteleport"), NOLABEL)
            {}

            void onClick(Click& click) override
            {
                click.ensureScriptThread([] {
                    const int ped = PLAYER::GET_PLAYER_PED(-1);
                    Vector3 cam_pos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
                    Vector3 cam_rot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
                    const float pitch = cam_rot.x * 0.01745329f;
                    const float yaw   = cam_rot.z * 0.01745329f;
                    const float cp    = cosf(pitch);
                    Vector3 dir{ cp * sinf(-yaw), cp * cosf(-yaw), sinf(pitch) };
                    const float dist  = 300.0f;
                    int test = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(
                        cam_pos.x, cam_pos.y, cam_pos.z,
                        cam_pos.x + dir.x * dist,
                        cam_pos.y + dir.y * dist,
                        cam_pos.z + dir.z * dist,
                        1, ped, 7
                    );
                    BOOL hit = FALSE;
                    Vector3 hit_pos{}, surface{};
                    Entity hit_ent = 0;
                    SHAPETEST::GET_SHAPE_TEST_RESULT(test, &hit, &hit_pos, &surface, &hit_ent);
                    if (hit)
                        ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, hit_pos.x, hit_pos.y, hit_pos.z + 1.0f, TRUE, TRUE, TRUE);
                    else
                        ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, cam_pos.x, cam_pos.y, cam_pos.z, TRUE, TRUE, TRUE);
                });
            }
        };

        class EditorClear : public CommandPhysical
        {
        public:
            explicit EditorClear(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Delete All Spawned"),
                    CMDNAMES("editorclear"), NOLABEL)
            {}

            void onClick(Click& click) override
            {
                auto* editor = parent->parent->as<CommandListEditor>();
                editor->children.erase(
                    editor->children.begin() + (std::ptrdiff_t)editor->spawned_offset,
                    editor->children.end()
                );
                editor->spawnedVersion.fetch_add(1, std::memory_order_relaxed);
            }
        };

        static void destroyPreview()
        {
            if (s_preview_handle) {
                int h = s_preview_handle;
                s_preview_handle = 0;
                ENTITY::SET_ENTITY_AS_MISSION_ENTITY(h, FALSE, TRUE);
                ENTITY::DELETE_ENTITY(&h);
            }
        }

        static void drawPreviewBox(int handle, joaat_t hash)
        {
            Vector3 mn{}, mx{};
            MISC::GET_MODEL_DIMENSIONS(hash, &mn, &mx);
            Vector3 pos = ENTITY::GET_ENTITY_COORDS(handle, FALSE);
            float dx = (mx.x - mn.x) * 0.5f;
            float dy = (mx.y - mn.y) * 0.5f;
            float dz = (mx.z - mn.z) * 0.5f;
            float ox = (mn.x + mx.x) * 0.5f;
            float oy = (mn.y + mx.y) * 0.5f;
            float oz = (mn.z + mx.z) * 0.5f;
            float cx = pos.x + ox, cy = pos.y + oy, cz = pos.z + oz;
            int r = 255, g = 0, b = 220, a = 255;
            // bottom face
            GRAPHICS::DRAW_LINE(cx-dx,cy-dy,cz-dz, cx+dx,cy-dy,cz-dz, r,g,b,a);
            GRAPHICS::DRAW_LINE(cx+dx,cy-dy,cz-dz, cx+dx,cy+dy,cz-dz, r,g,b,a);
            GRAPHICS::DRAW_LINE(cx+dx,cy+dy,cz-dz, cx-dx,cy+dy,cz-dz, r,g,b,a);
            GRAPHICS::DRAW_LINE(cx-dx,cy+dy,cz-dz, cx-dx,cy-dy,cz-dz, r,g,b,a);
            // top face
            GRAPHICS::DRAW_LINE(cx-dx,cy-dy,cz+dz, cx+dx,cy-dy,cz+dz, r,g,b,a);
            GRAPHICS::DRAW_LINE(cx+dx,cy-dy,cz+dz, cx+dx,cy+dy,cz+dz, r,g,b,a);
            GRAPHICS::DRAW_LINE(cx+dx,cy+dy,cz+dz, cx-dx,cy+dy,cz+dz, r,g,b,a);
            GRAPHICS::DRAW_LINE(cx-dx,cy+dy,cz+dz, cx-dx,cy-dy,cz+dz, r,g,b,a);
            // verticals
            GRAPHICS::DRAW_LINE(cx-dx,cy-dy,cz-dz, cx-dx,cy-dy,cz+dz, r,g,b,a);
            GRAPHICS::DRAW_LINE(cx+dx,cy-dy,cz-dz, cx+dx,cy-dy,cz+dz, r,g,b,a);
            GRAPHICS::DRAW_LINE(cx+dx,cy+dy,cz-dz, cx+dx,cy+dy,cz+dz, r,g,b,a);
            GRAPHICS::DRAW_LINE(cx-dx,cy+dy,cz-dz, cx-dx,cy+dy,cz+dz, r,g,b,a);
        }

        class EditorPreviewTick : public CommandPhysical
        {
            const char* m_last_model = nullptr;
            uint32_t m_lastFocusCount = 0;
            int m_noFocusFrames = 0;
            bool m_editorWasActive = false;
        public:
            EditorFreecam* m_freecam = nullptr;
            std::atomic<Rendering::Grid*> m_editorGrid{nullptr};

            explicit EditorPreviewTick(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, NOLABEL, CMDNAMES_0(), NOLABEL,
                    CMDFLAG_CONCEALED)
            { CommandTickDispatch::AddCommand(this); }

            ~EditorPreviewTick() override
            {
                CommandTickDispatch::RemoveCommand(this);
                destroyPreview();
                s_preview_model = nullptr;
            }

            void onTick() override
            {
                Rendering::Grid* editorGrid = m_editorGrid.load(std::memory_order_relaxed);
                const bool editorActive = GUI::IsOpen() && editorGrid &&
                    Rendering::MenuNavigation::IsDescendantActive(editorGrid);
                if (editorActive && !m_editorWasActive) {
                    m_editorWasActive = true;
                    if (m_freecam && !s_cam) {
                        m_freecam->m_on = true;
                        const int ped = PLAYER::GET_PLAYER_PED(-1);
                        const Vector3 pp = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
                        s_cam = CAMERA::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", TRUE);
                        CAMERA::SET_CAM_COORD(s_cam, pp.x, pp.y, pp.z + 5.0f);
                        CAMERA::SET_CAM_ROT(s_cam, -30.0f, 0.0f, 0.0f, 2);
                        CAMERA::SET_CAM_FOV(s_cam, 60.0f);
                        CAMERA::SET_CAM_ACTIVE(s_cam, TRUE);
                        CAMERA::RENDER_SCRIPT_CAMS(TRUE, FALSE, 0, TRUE, FALSE, 0);
                    }
                } else if (!editorActive && m_editorWasActive) {
                    m_editorWasActive = false;
                    if (m_freecam) m_freecam->m_on = false;
                    if (s_cam) {
                        CAMERA::SET_CAM_ACTIVE(s_cam, FALSE);
                        CAMERA::RENDER_SCRIPT_CAMS(FALSE, TRUE, 0, FALSE, FALSE, 0);
                        CAMERA::DESTROY_CAM(s_cam, FALSE);
                        s_cam = 0;
                    }
                }

                const uint32_t focusCount = s_virtualFocusCounter.load(std::memory_order_relaxed);
                if (focusCount != m_lastFocusCount) {
                    m_lastFocusCount = focusCount;
                    m_noFocusFrames = 0;
                } else if (++m_noFocusFrames > 5) {
                    s_preview_model = nullptr;
                    m_noFocusFrames = 0;
                    s_spawnerLeaveCount.fetch_add(1, std::memory_order_relaxed);
                }

                const char* want = s_preview_model;

                if (!want) {
                    if (s_preview_handle) {
                        destroyPreview();
                        m_last_model = nullptr;
                    }
                    return;
                }

                const joaat_t hash = Joaat(want);

                if (want != m_last_model) {
                    destroyPreview();
                    m_last_model = want;
                    s_preview_heading = 0.0f;
                }

                if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash)) {
                    destroyPreview();
                    m_last_model = nullptr;
                    return;
                }

                if (!STREAMING::HAS_MODEL_LOADED(hash)) {
                    STREAMING::REQUEST_MODEL(hash);
                    return;
                }

                if (PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_CONTEXT_SECONDARY))
                    s_preview_heading += 1.0f;
                if (PAD::IS_DISABLED_CONTROL_PRESSED(2, (int)ControllerInputs::INPUT_CONTEXT))
                    s_preview_heading -= 1.0f;

                Vector3 cam_pos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
                Vector3 cam_rot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
                const float yaw   = cam_rot.z * 0.01745329f;
                const float pitch = cam_rot.x * 0.01745329f;
                const float cp    = cosf(pitch);
                const float fx    = cp * sinf(-yaw);
                const float fy    = cp * cosf(-yaw);
                const float fz    = sinf(pitch);

                int test = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(
                    cam_pos.x, cam_pos.y, cam_pos.z,
                    cam_pos.x + fx * 100.0f,
                    cam_pos.y + fy * 100.0f,
                    cam_pos.z + fz * 100.0f,
                    1 | 16, s_preview_handle, 7
                );
                BOOL hit = FALSE;
                Vector3 place_pos{}, surface{};
                Entity hit_ent = 0;
                SHAPETEST::GET_SHAPE_TEST_RESULT(test, &hit, &place_pos, &surface, &hit_ent);
                if (!hit)
                    place_pos = { cam_pos.x + fx * 5.0f, cam_pos.y + fy * 5.0f, cam_pos.z + fz * 5.0f };

                if (!s_preview_handle) {
                    s_preview_handle = OBJECT::CREATE_OBJECT(hash, place_pos.x, place_pos.y, place_pos.z, TRUE, FALSE, FALSE);
                    if (s_preview_handle) {
                        ENTITY::SET_ENTITY_HAS_GRAVITY(s_preview_handle, FALSE);
                        ENTITY::FREEZE_ENTITY_POSITION(s_preview_handle, TRUE);
                        ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(s_preview_handle, FALSE, FALSE);
                        ENTITY::SET_ENTITY_ALPHA(s_preview_handle, 200, FALSE);
                    }
                    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
                    return;
                }

                ENTITY::SET_ENTITY_COORDS_NO_OFFSET(s_preview_handle,
                    place_pos.x, place_pos.y, place_pos.z,
                    FALSE, FALSE, FALSE);
                ENTITY::SET_ENTITY_ROTATION(s_preview_handle, 0.0f, 0.0f, s_preview_heading, 2, TRUE);

                drawPreviewBox(s_preview_handle, hash);
            }
        };

        void EditorActivePresence::fillVirtualItems(
            std::vector<std::unique_ptr<Rendering::GridItem>>& items, Rendering::Grid* grid)
        {
            m_tick->m_editorGrid.store(grid, std::memory_order_relaxed);
            auto* editor = parent->as<CommandListEditor>();
            const int ver = editor->spawnedVersion.load(std::memory_order_relaxed);
            grid->watchCondition([editor, ver]() -> bool {
                return editor->spawnedVersion.load(std::memory_order_relaxed) != ver;
            });
            struct ZeroItem : Rendering::GridItem {
                ZeroItem() : Rendering::GridItem(Rendering::GRIDITEM_INDIFFERENT, 0, 0) {}
                void draw() override {}
            };
            items.push_back(std::make_unique<ZeroItem>());
        }

        class EditorSpawnObject : public CommandPhysical
        {
        public:
            const char* m_model;
        private:
            CommandListEditor* m_editor;
        public:
            EditorSpawnObject(CommandList* parent, CommandListEditor* editor, const char* model)
                : CommandPhysical(COMMAND_ACTION, parent,
                    Label(model, Label::TagLiteral{}), CMDNAMES_0(), NOLABEL)
                , m_model(model)
                , m_editor(editor)
            {}

            ~EditorSpawnObject() override
            {
                if (s_preview_model == m_model) {
                    s_preview_model = nullptr;
                }
            }

            void onFocus() override { s_preview_model = m_model; }
            void onBlur()  override { if (s_preview_model == m_model) s_preview_model = nullptr; }

            void onClick(Click& click) override
            {
                const joaat_t hash   = Joaat(m_model);
                const char*   model  = m_model;
                auto*         editor = m_editor;

                const float heading = s_preview_heading;
                click.ensureScriptThread([hash, model, editor, heading] {
                    if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash))
                        return;
                    for (int i = 0; !STREAMING::HAS_MODEL_LOADED(hash); ++i) {
                        STREAMING::REQUEST_MODEL(hash);
                        Script::current()->yield();
                        if (i > 30) return;
                    }
                    Vector3 spawn_pos;
                    if (s_preview_handle) {
                        spawn_pos = ENTITY::GET_ENTITY_COORDS(s_preview_handle, FALSE);
                        destroyPreview();
                    } else {
                        spawn_pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(-1), TRUE);
                    }
                    int obj = OBJECT::CREATE_OBJECT(hash, spawn_pos.x, spawn_pos.y, spawn_pos.z, TRUE, FALSE, TRUE);
                    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
                    if (obj) {
                        ENTITY::SET_ENTITY_ROTATION(obj, 0.0f, 0.0f, heading, 2, TRUE);
                        editor->createChild<EditorSpawnedObject>(model, obj);
                        editor->spawnedVersion.fetch_add(1, std::memory_order_relaxed);
                    }
                });
            }
        };

        class EditorSpawnAny : public CommandPhysical
        {
            CommandListEditor* m_editor;
        public:
            EditorSpawnAny(CommandList* parent, CommandListEditor* editor)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Input Model Name"),
                    CMDNAMES("spawnobject"),
                    LIT("The \"expert mode\" way of spawning objects."))
                , m_editor(editor)
            {}

            void onClick(Click& click) override
            {
                auto* editor = m_editor;
                Rendering::MenuCommandBox::Open(
                    "spawnobject",
                    "Input Model Name - The \"expert mode\" way of spawning objects.",
                    "",
                    "",
                    [editor](const std::string& text) -> bool {
                        if (text.empty()) return false;
                        std::string model = text;
                        const joaat_t hash = Joaat(model);
                        FiberPool::queueJob([hash, model, editor] {
                            if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash)) return;
                            for (int i = 0; !STREAMING::HAS_MODEL_LOADED(hash); ++i) {
                                STREAMING::REQUEST_MODEL(hash);
                                Script::current()->yield();
                                if (i > 30) return;
                            }
                            Vector3 pp = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(-1), TRUE);
                            int obj = OBJECT::CREATE_OBJECT(hash, pp.x, pp.y, pp.z, TRUE, FALSE, TRUE);
                            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
                            if (obj) {
                                editor->createChild<EditorSpawnedObject>(model.c_str(), obj);
                                editor->spawnedVersion.fetch_add(1, std::memory_order_relaxed);
                            }
                        });
                        return true;
                    }
                );
            }

            void onCommand(Click& click, std::wstring& args) override
            {
                if (args.empty()) { onClick(click); return; }
                std::string name;
                for (wchar_t w : args)
                    name += (char)(w & 0x7F);
                args.clear();

                const joaat_t hash = Joaat(name);
                auto* editor = m_editor;
                const std::string model = name;

                click.ensureScriptThread([hash, model, editor] {
                    if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash)) return;
                    for (int i = 0; !STREAMING::HAS_MODEL_LOADED(hash); ++i) {
                        STREAMING::REQUEST_MODEL(hash);
                        Script::current()->yield();
                        if (i > 30) return;
                    }
                    Vector3 pp = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(-1), TRUE);
                    int obj = OBJECT::CREATE_OBJECT(hash, pp.x, pp.y, pp.z, TRUE, FALSE, TRUE);
                    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
                    if (obj) {
                        editor->createChild<EditorSpawnedObject>(model.c_str(), obj);
                        editor->spawnedVersion.fetch_add(1, std::memory_order_relaxed);
                    }
                });
            }
        };

        static constexpr int kWindowSize = 14;
        static constexpr int kObjTotal  = (int)std::size(g_objects);

        struct VirtualListState {
            int windowStart = 0;
            CommandListEditor* editor;
            std::vector<const char*> filteredObjects;
            Rendering::Grid* grid = nullptr;
            Command* search = nullptr;
            Command* spawnAny = nullptr;
            Command* objectsHeader = nullptr;
            Command* downtown = nullptr;

            explicit VirtualListState(CommandListEditor* e) : editor(e) {}

            int totalCount() const
            {
                return filteredObjects.empty() ? kObjTotal : (int)filteredObjects.size();
            }

            const char* getModel(int absIdx) const
            {
                if (!filteredObjects.empty())
                    return (absIdx >= 0 && absIdx < (int)filteredObjects.size()) ? filteredObjects[absIdx] : nullptr;
                return (absIdx >= 0 && absIdx < kObjTotal) ? g_objects[absIdx] : nullptr;
            }

            void updateScroll() const
            {
                const bool scrolled = windowStart > 0;
                auto applyConcealed = [scrolled](Command* cmd) {
                    if (!cmd) return;
                    if (scrolled) cmd->flags |= CMDFLAG_CONCEALED;
                    else          cmd->flags &= ~CMDFLAG_CONCEALED;
                };
                applyConcealed(search);
                applyConcealed(spawnAny);
                applyConcealed(objectsHeader);
                applyConcealed(downtown);
                if (!grid) return;
                const int total = totalCount();
                if (total <= kWindowSize) {
                    grid->setVirtualScroll(0, 0);
                } else {
                    const int32_t totalHeight = (int32_t)total * Rendering::Theme::kContentItemHeight;
                    const int16_t offset = (int16_t)(windowStart * Rendering::Theme::kContentItemHeight);
                    grid->setVirtualScroll(totalHeight, offset);
                }
            }
        };

        class GridItemVirtualRow : public Rendering::GridItem
        {
            std::shared_ptr<VirtualListState> m_state;
            int m_row;
            bool m_wasFocused = false;

            [[nodiscard]] int currentIdx() const { return m_state->windowStart + m_row; }

        public:
            GridItemVirtualRow(int16_t w, std::shared_ptr<VirtualListState> state, int row)
                : Rendering::GridItem(Rendering::GRIDITEM_INDIFFERENT, w,
                    state->getModel(state->windowStart + row) != nullptr
                        ? Rendering::Theme::kContentItemHeight : 0)
                , m_state(std::move(state)), m_row(row)
            {}

            [[nodiscard]] bool isFocusable() const override
            {
                return m_state->getModel(m_state->windowStart + m_row) != nullptr;
            }

            void draw() override
            {
                s_virtualFocusCounter.fetch_add(1, std::memory_order_relaxed);
                const bool focused = isKeyboardFocused();
                if (focused) {
                    const int idx = currentIdx();
                    s_preview_model = m_state->getModel(idx);
                    Rendering::GridRenderer::DrawRect(
                        (float)x, (float)y, (float)width,
                        (float)Rendering::Theme::kContentItemHeight,
                        Rendering::Theme::kAccent);
                } else if (m_wasFocused) {
                    const int idx = currentIdx();
                    const char* myModel = m_state->getModel(idx);
                    if (s_preview_model == myModel) s_preview_model = nullptr;
                }
                m_wasFocused = focused;
            }

            void drawText() override
            {
                const int idx = currentIdx();
                const char* model = m_state->getModel(idx);
                if (!model) return;
                const auto& col = isKeyboardFocused()
                    ? Rendering::Theme::kFocusText
                    : Rendering::Theme::kUnfocusedText;
                Rendering::GridRenderer::DrawText(
                    (float)(x + 4), (float)(y + 8), model, col);
            }

            bool handleNavigation(int delta) override
            {
                if (delta == 1 && m_row == kWindowSize - 1) {
                    const int newStart = m_state->windowStart + 1;
                    if (m_state->getModel(newStart + m_row) == nullptr) {
                        m_state->windowStart = 0;
                        m_state->updateScroll();
                        return false;
                    }
                    m_state->windowStart = newStart;
                    m_state->updateScroll();
                    return true;
                }
                if (delta == -1 && m_row == 0) {
                    if (m_state->windowStart == 0) return false;
                    m_state->windowStart--;
                    m_state->updateScroll();
                    return true;
                }
                return false;
            }

            void activate() override
            {
                const int idx = currentIdx();
                const char* model = m_state->getModel(idx);
                if (!model) return;
                const joaat_t hash = Joaat(model);
                auto* editor = m_state->editor;
                const float heading = s_preview_heading;
                FiberPool::queueJob([hash, model, editor, heading] {
                    if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash)) return;
                    for (int i = 0; !STREAMING::HAS_MODEL_LOADED(hash); ++i) {
                        STREAMING::REQUEST_MODEL(hash);
                        Script::current()->yield();
                        if (i > 30) return;
                    }
                    Vector3 spawnPos;
                    if (s_preview_handle) {
                        spawnPos = ENTITY::GET_ENTITY_COORDS(s_preview_handle, FALSE);
                        destroyPreview();
                    } else {
                        spawnPos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(-1), TRUE);
                    }
                    int obj = OBJECT::CREATE_OBJECT(hash, spawnPos.x, spawnPos.y, spawnPos.z, TRUE, FALSE, TRUE);
                    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
                    if (obj) {
                        ENTITY::SET_ENTITY_ROTATION(obj, 0.0f, 0.0f, heading, 2, TRUE);
                        editor->createChild<EditorSpawnedObject>(model, obj);
                        editor->spawnedVersion.fetch_add(1, std::memory_order_relaxed);
                    }
                });
            }
        };

        class CommandVirtualObjectList : public CommandPhysical
        {
            std::shared_ptr<VirtualListState> m_state;
            int m_lastLeaveSnapshot = -1;
        public:
            CommandVirtualObjectList(CommandList* parent, std::shared_ptr<VirtualListState> state)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Objects"), CMDNAMES_0(), NOLABEL)
                , m_state(std::move(state))
            {}

            void fillVirtualItems(std::vector<std::unique_ptr<Rendering::GridItem>>& items, Rendering::Grid* grid) override
            {
                m_state->grid = grid;
                const int leaveSnapshot = s_spawnerLeaveCount.load(std::memory_order_relaxed);
                if (m_lastLeaveSnapshot != leaveSnapshot) {
                    m_lastLeaveSnapshot = leaveSnapshot;
                    m_state->windowStart = 0;
                    m_state->filteredObjects.clear();
                }
                m_state->updateScroll();
                grid->watchCondition([leaveSnapshot]() -> bool {
                    return s_spawnerLeaveCount.load(std::memory_order_relaxed) != leaveSnapshot;
                });
                auto state = m_state;
                grid->watchCondition([state]() -> bool {
                    return state->windowStart == 0;
                });
                const size_t filteredSize = m_state->filteredObjects.size();
                grid->watchCondition([state, filteredSize]() -> bool {
                    return state->filteredObjects.size() != filteredSize;
                });
                for (int i = 0; i < kWindowSize; ++i)
                    items.push_back(std::make_unique<GridItemVirtualRow>(
                        Rendering::Theme::kContentWidth, m_state, i));
            }
        };

        class GridItemSearchButton : public Rendering::GridItem
        {
            std::shared_ptr<VirtualListState> m_state;
        public:
            GridItemSearchButton(int16_t w, int16_t h, std::shared_ptr<VirtualListState> state)
                : Rendering::GridItem(Rendering::GRIDITEM_INDIFFERENT, w, h)
                , m_state(std::move(state))
            {}

            [[nodiscard]] bool isFocusable() const override { return true; }

            bool handleNavigation(int delta) override
            {
                if (delta == -1) {
                    const int total = m_state->totalCount();
                    m_state->windowStart = std::max(0, total - kWindowSize);
                    m_state->updateScroll();
                }
                return false;
            }

            void draw() override
            {
                if (isKeyboardFocused())
                    Rendering::GridRenderer::DrawRect(
                        (float)x, (float)y, (float)width, (float)height,
                        Rendering::Theme::kAccent);
            }

            void drawText() override
            {
                const auto& col = isKeyboardFocused()
                    ? Rendering::Theme::kFocusText
                    : Rendering::Theme::kUnfocusedText;
                Rendering::GridRenderer::DrawText(
                    (float)(x + 4), (float)(y + 8), "Search...", col);
            }

            void activate() override
            {
                auto state = m_state;
                Rendering::MenuCommandBox::Open(
                    "findobject",
                    "Spawner: Search",
                    "",
                    "",
                    [state](const std::string& text) -> bool {
                        state->windowStart = 0;
                        state->filteredObjects.clear();
                        if (!text.empty()) {
                            const std::string key = simplified(text);
                            for (const auto& model : g_objects_downtown)
                                if (simplified(model).find(key) != std::string::npos)
                                    state->filteredObjects.push_back(model);
                            for (const auto& model : g_objects)
                                if (simplified(model).find(key) != std::string::npos)
                                    state->filteredObjects.push_back(model);
                        }
                        state->updateScroll();
                        return true;
                    }
                );
            }
        };

        class EditorSearch : public CommandPhysical
        {
            std::shared_ptr<VirtualListState> m_state;
        public:
            EditorSearch(CommandList* parent, std::shared_ptr<VirtualListState> state)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Search"), CMDNAMES("findobject"), NOLABEL)
                , m_state(std::move(state))
            {}

            void fillVirtualItems(std::vector<std::unique_ptr<Rendering::GridItem>>& items, Rendering::Grid*) override
            {
                items.push_back(std::make_unique<GridItemSearchButton>(
                    Rendering::Theme::kContentWidth,
                    Rendering::Theme::kContentItemHeight,
                    m_state));
            }
        };

        class CommandListObjectSpawner : public CommandList
        {
        public:
            explicit CommandListObjectSpawner(CommandList* parent, CommandListEditor* editor)
                : CommandList(parent, LIT("Spawner"))
            {
                auto state = std::make_shared<VirtualListState>(editor);

                state->search    = createChild<EditorSearch>(state);
                state->spawnAny  = createChild<EditorSpawnAny>(editor);
                state->objectsHeader = createChild<EditorSectionHeader>("Objects");

                {
                    auto* dt = createChild<CommandList>(LIT("Downtown"));
                    dt->children.reserve(std::size(g_objects_downtown));
                    for (const auto& model : g_objects_downtown)
                        dt->createChild<EditorSpawnObject>(editor, model);
                    state->downtown = dt;
                }

                createChild<CommandVirtualObjectList>(state);
            }
        };
    }

    CommandListEditor::CommandListEditor(CommandList* parent)
        : CommandList(parent, LIT("Editor"), CMDNAMES("editor"))
    {
        auto* tick = createChild<EditorPreviewTick>();
        createChild<EditorActivePresence>(tick);
        createChild<CommandListObjectSpawner>(this);

        {
            auto* tools = createChild<CommandList>(LIT("Tools"));
            tick->m_freecam = tools->createChild<EditorFreecam>();
            tools->createChild<EditorTeleport>();
            tools->createChild<EditorClear>();
        }

        createChild<EditorSectionHeader>("Spawned");
        spawned_offset = children.size();
    }
}
