#include "Rendering/StandPort/CommandColourCustom.hpp"

#include "Rendering/StandPort/CommandColourSlider.hpp"
#include "Commands/Widgets/CommandStateSerializer.hpp"
#include "Menu/Click.hpp"
#include "Rendering/StandPort/ColourUtil.hpp"
#include "Rendering/StandPort/CommandDivider.hpp"
#include "Rendering/StandPort/CommandColourHex.hpp"
#include "Rendering/StandPort/CommandCurrentCustomColourHex.hpp"
#include "Rendering/StandPort/CommandLambdaAction.hpp"
#include "Rendering/StandPort/get_next_arg.hpp"
#include "lib/soup/Rgb.hpp"
#include "lib/soup/unicode.hpp"

#include <format>
#include <memory>

namespace Stand
{
	CommandColourCustom::CommandColourCustom(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, int default_r, int default_g, int default_b, int default_a, commandflags_t flags)
		: CommandList(parent, std::move(menu_name), std::move(command_names), std::move(help_text), flags, COMMAND_LIST_COLOUR)
	{
		h = createChild<CommandColourSlider>(LIT("Hue"),        getSuffixedCommandNames("hue"),        CommandColourSlider::HSV,     0, 359, 0);
		s = createChild<CommandColourSlider>(LIT("Saturation"), getSuffixedCommandNames("saturation"), CommandColourSlider::HSV,     0, 100, 0);
		v = createChild<CommandColourSlider>(LIT("Value"),      getSuffixedCommandNames("value"),      CommandColourSlider::HSV,     0, 100, 0);
		if (default_a != -1)
		{
			a = createChild<CommandColourSlider>(LIT("Opacity"), getSuffixedCommandNames("opacity"), CommandColourSlider::OPACITY, 0, 255, default_a);
		}
		r = createChild<CommandColourSlider>(LIT("Red"),   getSuffixedCommandNames("red"),   CommandColourSlider::RGB, 0, 255, default_r);
		g = createChild<CommandColourSlider>(LIT("Green"), getSuffixedCommandNames("green"), CommandColourSlider::RGB, 0, 255, default_g);
		b = createChild<CommandColourSlider>(LIT("Blue"),  getSuffixedCommandNames("blue"),  CommandColourSlider::RGB, 0, 255, default_b);
		updateHSVRaw();
	}

	CommandColourCustom::CommandColourCustom(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, const DirectX::SimpleMath::Color& default_rgba, bool transparency, commandflags_t flags)
		: CommandColourCustom(parent, std::move(menu_name), std::move(command_names), std::move(help_text), (int)(default_rgba.x * 255.0f), (int)(default_rgba.y * 255.0f), (int)(default_rgba.z * 255.0f), transparency ? (int)(default_rgba.w * 255.0f) : -1, flags)
	{
	}

	void CommandColourCustom::populate()
	{
		children.insert(children.begin(), std::make_unique<CommandDivider>(this, LOC("CR_HSV")));
		if (a != nullptr)
		{
			children.insert(children.begin() + 4, std::make_unique<CommandDivider>(this, LOC("OPCTY")));
			children.insert(children.begin() + 6, std::make_unique<CommandDivider>(this, LOC("CR_RGB")));
		}
		else
		{
			children.insert(children.begin() + 4, std::make_unique<CommandDivider>(this, LOC("CR_RGB")));
		}
		updateHSVRaw();
		default_value = getRGBAInt();
		createChild<CommandDivider>(LOC("OTHR"));
		if (!command_names.empty())
			createChild<CommandColourHex>();
		createChild<CommandCurrentCustomColourHex>(this);
	}

	void CommandColourCustom::populateWithOptionalCopyFrom(CommandColourCustom* const copy_from)
	{
		populate();
	}

	void CommandColourCustom::populate(CommandColourCustom* const copy_from)
	{
		populate();
		createCopyFrom(copy_from);
	}

	void CommandColourCustom::createCopyFrom(CommandColourCustom* const copy_from)
	{
		createChild<CommandLambdaAction>(LIT("Copy from: " + copy_from->menu_name.getLocalisedUtf8()), std::vector<CommandName>{}, NOLABEL, [this, copy_from](Click& click)
		{
			this->set(click, copy_from->getRGBA());
		});
	}

	void CommandColourCustom::onCommand(Click& click, std::wstring& args)
	{
		auto arg = get_next_arg(args);
		if (arg.empty())
		{
			CommandList::onClick(click);
		}
		else
		{
			try
			{
				setState(click, soup::unicode::utf16_to_utf8(arg));
			}
			catch (const std::exception&)
			{
				click.setResponse(LOC("INVARG"));
				std::wstring prefill;
				if (!command_names.empty())
					prefill = cmdNameToUtf16(command_names.at(0));
				prefill.push_back(L' ');
				prefill.append(arg);
				click.showCommandBoxIfPossible(std::move(prefill));
			}
		}
	}

	void CommandColourCustom::onChange(Click& click)
	{
	}

	void CommandColourCustom::updateRGB()
	{
		const DirectX::SimpleMath::Vector3 rgb = ColourUtil::hsv_to_rgb(getHSV());
		Click click(CLICK_BULK, TC_OTHER);
		this->r->setValue(click, (int)(rgb.x * 255.0f));
		this->g->setValue(click, (int)(rgb.y * 255.0f));
		this->b->setValue(click, (int)(rgb.z * 255.0f));
	}

	void CommandColourCustom::updateHSV()
	{
		Click click(CLICK_BULK, TC_OTHER);
		updateHSV(click);
	}

	void CommandColourCustom::updateHSV(Click& click)
	{
		const DirectX::SimpleMath::Vector3 hsv = ColourUtil::rgb_to_hsv(getRGB());
		this->h->setValue(click, (int)hsv.x);
		this->s->setValue(click, (int)(hsv.y * 100.0f));
		this->v->setValue(click, (int)(hsv.z * 100.0f));
	}

	void CommandColourCustom::updateHSVRaw()
	{
		const DirectX::SimpleMath::Vector3 hsv = ColourUtil::rgb_to_hsv(getRGB());
		this->h->value = (int)hsv.x;
		this->s->value = (int)(hsv.y * 100.0f);
		this->v->value = (int)(hsv.z * 100.0f);
	}

	void CommandColourCustom::getRGB(int& out_r, int& out_g, int& out_b) const
	{
		out_r = this->r->value;
		out_g = this->g->value;
		out_b = this->b->value;
	}

	DirectX::SimpleMath::Vector3 CommandColourCustom::getRGB() const
	{
		return {
			(float)this->r->value / 255.0f,
			(float)this->g->value / 255.0f,
			(float)this->b->value / 255.0f
		};
	}

	DirectX::SimpleMath::Vector3 CommandColourCustom::getHSV() const
	{
		return {
			(float)this->h->value,
			(float)this->s->value / 100.0f,
			(float)this->v->value / 100.0f
		};
	}

	float CommandColourCustom::getAlpha() const
	{
		return (this->a == nullptr ? 1.0f : float(this->a->value) / 255.0f);
	}

	DirectX::SimpleMath::Color CommandColourCustom::getRGBA() const
	{
		return {
			(float)this->r->value / 255.0f,
			(float)this->g->value / 255.0f,
			(float)this->b->value / 255.0f,
			getAlpha()
		};
	}

	uint32_t CommandColourCustom::getRGBAInt() const
	{
		uint32_t rgba = this->r->value;
		rgba <<= 8;
		rgba |= this->g->value;
		rgba <<= 8;
		rgba |= this->b->value;
		rgba <<= 8;
		if (this->a)
		{
			rgba |= this->a->value;
		}
		return rgba;
	}

	std::string CommandColourCustom::getHex() const
	{
		return std::format("{:08X}", getRGBAInt());
	}

	void CommandColourCustom::set(Click& click, uint32_t rgba)
	{
		Click click_ = click.derive(CLICK_BULK);
		if (this->a != nullptr)
		{
			this->a->setValue(click_, rgba & 0xFF);
		}
		rgba >>= 8;
		this->b->setValue(click_, rgba & 0xFF);
		rgba >>= 8;
		this->g->setValue(click_, rgba & 0xFF);
		rgba >>= 8;
		this->r->setValue(click_, rgba);
		updateHSV(click_);
		processChange(click);
	}

	void CommandColourCustom::set(Click& click, int in_r, int in_g, int in_b)
	{
		this->r->setValue(click, in_r);
		this->g->setValue(click, in_g);
		this->b->setValue(click, in_b);
		updateHSV();
		updateState(click.type);
	}

	void CommandColourCustom::set(Click& click, const DirectX::SimpleMath::Color& rgba)
	{
		Click click_ = click.derive(CLICK_BULK);
		this->r->setValue(click_, (int)(rgba.R() * 255.0f));
		this->g->setValue(click_, (int)(rgba.G() * 255.0f));
		this->b->setValue(click_, (int)(rgba.B() * 255.0f));
		if (this->a != nullptr)
		{
			this->a->setValue(click_, (int)(rgba.A() * 255.0f));
		}
		updateHSV();
		processChange(click);
	}

	void CommandColourCustom::processChange(Click& click)
	{
		updateState(click.type);
		onChange(click);
	}

	void CommandColourCustom::updateState(ClickType click_type)
	{
		if (click_type != CLICK_BULK && supportsStateOperations())
		{
			CommandStateSerializer::MarkDirty();
		}
	}

	std::string CommandColourCustom::getState() const
	{
		return std::format("{:08X}", getRGBAInt());
	}

	std::string CommandColourCustom::getDefaultState() const
	{
		return std::format("{:08X}", default_value);
	}

	void CommandColourCustom::setState(Click& click, const std::string& _state)
	{
		if (!_state.empty())
		{
			std::string state(_state);
			if (state.size() == 3 || state.size() == 6)
			{
				if (state.size() == 3)
				{
					state = std::string(2, state[0]) + std::string(2, state[1]) + std::string(2, state[2]);
				}
				state += "FF";
			}
			uint32_t rgba = std::stoul(state, nullptr, 16);
			if (6 >= _state.size())
			{
				rgba <<= 8;
				rgba |= 0xFF;
			}
			set(click, rgba);
		}
		else
		{
			applyDefaultState();
		}
	}

	void CommandColourCustom::applyDefaultState()
	{
		Click click(CLICK_BULK, TC_APPLYDEFAULTSTATE);
		set(click, default_value);
	}

	std::vector<CommandName> CommandColourCustom::getRainbowCommandNames() const
	{
		std::vector<CommandName> res{};
		if (!command_names.empty())
		{
			res.emplace_back(command_names.at(0) + "rainbow");
		}
		return res;
	}
}
