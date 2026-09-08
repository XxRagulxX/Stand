#include "Rendering/TransactionsGrid.hpp"

#include "AntiCheat/AnticheatBypass.hpp"
#include "Core/Pointers.hpp"
#include "Network/CNetShopTransaction.hpp"
#include "Network/netCatalogBaseItem.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemIntStepper.hpp"
#include "Rendering/GridItemLocalList.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Invoker.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Scripting/Scripts.hpp"

#include <array>
#include <format>
#include <limits>
#include <unordered_set>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Duplicated from World/Transactions.cpp's own file-static
		// tables (internal linkage there, so unreachable from here) -
		// same "mirror the original's logic, don't reuse it" approach as
		// every other port in this series.
		constexpr std::array<std::pair<const char*, std::uint32_t>, 28> kNetShopActions = {{
		    {"ACQUIRE", "NET_SHOP_ACTION_ACQUIRE"_J},
		    {"ADD_CONTRABAND", "NET_SHOP_ACTION_ADD_CONTRABAND"_J},
		    {"ALLOT", "NET_SHOP_ACTION_ALLOT"_J},
		    {"BUY_CASINO_CHIPS", "NET_SHOP_ACTION_BUY_CASINO_CHIPS"_J},
		    {"BUY_CONTRABAND_MISSION", "NET_SHOP_ACTION_BUY_CONTRABAND_MISSION"_J},
		    {"BUY_ITEM", "NET_SHOP_ACTION_BUY_ITEM"_J},
		    {"BUY_PROPERTY", "NET_SHOP_ACTION_BUY_PROPERTY"_J},
		    {"BUY_UNLOCK", "NET_SHOP_ACTION_BUY_UNLOCK"_J},
		    {"BUY_VEHICLE", "NET_SHOP_ACTION_BUY_VEHICLE"_J},
		    {"BUY_VEHICLE_MODS", "NET_SHOP_ACTION_BUY_VEHICLE_MODS"_J},
		    {"BUY_WAREHOUSE", "NET_SHOP_ACTION_BUY_WAREHOUSE"_J},
		    {"CREATE_PLAYER_APPEARANCE", "NET_SHOP_ACTION_CREATE_PLAYER_APPEARANCE"_J},
		    {"DELETE_CHAR", "NET_SHOP_ACTION_DELETE_CHAR"_J},
		    {"EARN", "NET_SHOP_ACTION_EARN"_J},
		    {"EARN_LIMITED_SERVICE", "NET_SHOP_ACTION_EARN_LIMITED_SERVICE"_J},
		    {"GIVE", "NET_SHOP_ACTION_GIVE"_J},
		    {"PURCH", "NET_SHOP_ACTION_PURCH"_J},
		    {"RECOUP", "NET_SHOP_ACTION_RECOUP"_J},
		    {"REMOVE_CONTRABAND", "NET_SHOP_ACTION_REMOVE_CONTRABAND"_J},
		    {"RESET_BUSINESS_PROGRESS", "NET_SHOP_ACTION_RESET_BUSINESS_PROGRESS"_J},
		    {"SELL_CASINO_CHIPS", "NET_SHOP_ACTION_SELL_CASINO_CHIPS"_J},
		    {"SELL_VEHICLE", "NET_SHOP_ACTION_SELL_VEHICLE"_J},
		    {"SPEND", "NET_SHOP_ACTION_SPEND"_J},
		    {"SPEND_LIMITED_SERVICE", "NET_SHOP_ACTION_SPEND_LIMITED_SERVICE"_J},
		    {"UPDATE_BUSINESS_GOODS", "NET_SHOP_ACTION_UPDATE_BUSINESS_GOODS"_J},
		    {"UPDATE_STORAGE_DATA", "NET_SHOP_ACTION_UPDATE_STORAGE_DATA"_J},
		    {"UPDATE_WAREHOUSE_VEHICLE", "NET_SHOP_ACTION_UPDATE_WAREHOUSE_VEHICLE"_J},
		    {"USE", "NET_SHOP_ACTION_USE"_J},
		}};

		constexpr std::array<std::pair<const char*, std::uint32_t>, 60> kNetShopCategories = {{
		    {"BEARD", "CATEGORY_BEARD"_J},
		    {"BLUSHER", "CATEGORY_BLUSHER"_J},
		    {"CASINO_CHIP_REASON", "CATEGORY_CASINO_CHIP_REASON"_J},
		    {"CASINO_CHIPS", "CATEGORY_CASINO_CHIPS"_J},
		    {"CHEST_HAIR", "CATEGORY_CHEST_HAIR"_J},
		    {"CLOTH", "CATEGORY_CLOTH"_J},
		    {"CONTACTS", "CATEGORY_CONTACTS"_J},
		    {"CONTRABAND_FLAGS", "CATEGORY_CONTRABAND_FLAGS"_J},
		    {"CONTRABAND_MISSION", "CATEGORY_CONTRABAND_MISSION"_J},
		    {"CONTRABAND_QNTY", "CATEGORY_CONTRABAND_QNTY"_J},
		    {"CURRENCY_TYPE", "CATEGORY_CURRENCY_TYPE"_J},
		    {"DATA_STORAGE", "CATEGORY_DATA_STORAGE"_J},
		    {"DECORATION", "CATEGORY_DECORATION"_J},
		    {"EARN_CURRENCY", "CATEGORY_EARN_CURRENCY"_J},
		    {"EYEBROWS", "CATEGORY_EYEBROWS"_J},
		    {"FACEPAINT", "CATEGORY_FACEPAINT"_J},
		    {"HAIR", "CATEGORY_HAIR"_J},
		    {"INVENTORY_BEARD", "CATEGORY_INVENTORY_BEARD"_J},
		    {"INVENTORY_BLUSHER", "CATEGORY_INVENTORY_BLUSHER"_J},
		    {"INVENTORY_CHEST_HAIR", "CATEGORY_INVENTORY_CHEST_HAIR"_J},
		    {"INVENTORY_CONTACTS", "CATEGORY_INVENTORY_CONTACTS"_J},
		    {"INVENTORY_CONTRABAND_MISSION", "CATEGORY_INVENTORY_CONTRABAND_MISSION"_J},
		    {"INVENTORY_CURRENCY", "CATEGORY_INVENTORY_CURRENCY"_J},
		    {"INVENTORY_EYEBROWS", "CATEGORY_INVENTORY_EYEBROWS"_J},
		    {"INVENTORY_FACEPAINT", "CATEGORY_INVENTORY_FACEPAINT"_J},
		    {"INVENTORY_HAIR", "CATEGORY_INVENTORY_HAIR"_J},
		    {"INVENTORY_ITEM", "CATEGORY_INVENTORY_ITEM"_J},
		    {"INVENTORY_LIPSTICK", "CATEGORY_INVENTORY_LIPSTICK"_J},
		    {"INVENTORY_MKUP", "CATEGORY_INVENTORY_MKUP"_J},
		    {"INVENTORY_PRICE_PAID", "CATEGORY_INVENTORY_PRICE_PAID"_J},
		    {"INVENTORY_PROPERTY", "CATEGORY_INVENTORY_PROPERTIE"_J},
		    {"INVENTORY_PROPERTY_INTERIOR", "CATEGORY_INVENTORY_PROPERTY_INTERIOR"_J},
		    {"INVENTORY_VEHICLE", "CATEGORY_INVENTORY_VEHICLE"_J},
		    {"INVENTORY_VEHICLE_MOD", "CATEGORY_INVENTORY_VEHICLE_MOD"_J},
		    {"INVENTORY_WAREHOUSE", "CATEGORY_INVENTORY_WAREHOUSE"_J},
		    {"INVENTORY_WAREHOUSE_INTERIOR", "CATEGORY_INVENTORY_WAREHOUSE_INTERIOR"_J},
		    {"LIPSTICK", "CATEGORY_LIPSTICK"_J},
		    {"MART", "CATEGORY_MART"_J},
		    {"MKUP", "CATEGORY_MKUP"_J},
		    {"PRICE_MODIFIER", "CATEGORY_PRICE_MODIFIER"_J},
		    {"PRICE_OVERRIDE", "CATEGORY_PRICE_OVERRIDE"_J},
		    {"PROPERTY", "CATEGORY_PROPERTIE"_J},
		    {"PROPERTY_INTERIOR", "CATEGORY_PROPERTY_INTERIOR"_J},
		    {"SERVICE", "CATEGORY_SERVICE"_J},
		    {"SERVICE_UNLOCKED", "CATEGORY_SERVICE_UNLOCKED"_J},
		    {"SERVICE_WITH_LIMIT", "CATEGORY_SERVICE_WITH_LIMIT"_J},
		    {"SERVICE_WITH_THRESHOLD", "CATEGORY_SERVICE_WITH_THRESHOLD"_J},
		    {"SYSTEM", "CATEGORY_SYSTEM"_J},
		    {"TATTOO", "CATEGORY_TATTOO"_J},
		    {"UNLOCK", "CATEGORY_UNLOCK"_J},
		    {"VEHICLE", "CATEGORY_VEHICLE"_J},
		    {"VEHICLE_MOD", "CATEGORY_VEHICLE_MOD"_J},
		    {"VEHICLE_UPGRADE", "CATEGORY_VEHICLE_UPGRADE"_J},
		    {"VENDING_MACHINE", "CATEGORY_VENDING_MACHINE"_J},
		    {"WAREHOUSE", "CATEGORY_WAREHOUSE"_J},
		    {"WAREHOUSE_INTERIOR", "CATEGORY_WAREHOUSE_INTERIOR"_J},
		    {"WAREHOUSE_VEHICLE_INDEX", "CATEGORY_WAREHOUSE_VEHICLE_INDEX"_J},
		    {"WEAPON", "CATEGORY_WEAPON"_J},
		    {"WEAPON_AMMO", "CATEGORY_WEAPON_AMMO"_J},
		    {"WEAPON_MOD", "CATEGORY_WEAPON_MOD"_J},
		}};

		const std::unordered_set<std::uint32_t> kBannedItemHashes = {
		    "SERVICE_EARN_DEBUG"_J,
		    "SERVICE_EARN_JBONUS"_J,
		    "SERVICE_EARN_JBONUS_MODEL"_J,
		    "SERVICE_EARN_JBONUS_NO_DEATH"_J,
		    "SERVICE_EARN_JBONUS_NO_FALL"_J,
		    "SERVICE_EARN_JBONUS_NOT_SEEN"_J,
		    "SERVICE_EARN_JBONUS_SE"_J,
		    "SERVICE_EARN_EVOLVE"_J,
		    "SERVICE_EARN_IMPULSE"_J,
		};

		const char* CategoryNameFromHash(std::uint32_t hash)
		{
			for (auto& cat : kNetShopCategories)
				if (cat.second == hash)
					return cat.first;

			return "UNKNOWN";
		}

		bool IsCategoryService(std::uint32_t hash)
		{
			return hash == "CATEGORY_SERVICE"_J || hash == "CATEGORY_SERVICE_WITH_LIMIT"_J || hash == "CATEGORY_SERVICE_WITH_THRESHOLD"_J;
		}

		bool IsActionService(std::uint32_t hash)
		{
			return hash == "NET_SHOP_ACTION_EARN"_J || hash == "NET_SHOP_ACTION_SPEND"_J;
		}

		bool IsPriceModifier(std::uint32_t hash)
		{
			return hash == "CATEGORY_PRICE_MODIFIER"_J || hash == "CATEGORY_PRICE_OVERRIDE"_J;
		}

		bool ShouldSwapItemParams(std::uint32_t hash)
		{
			switch (hash)
			{
			case "CATEGORY_INVENTORY_ITEM"_J:
			case "CATEGORY_INVENTORY_VEHICLE"_J:
			case "CATEGORY_INVENTORY_VEHICLE_MOD"_J:
			case "CATEGORY_INVENTORY_PROPERTIE"_J:
			case "CATEGORY_INVENTORY_BEARD"_J:
			case "CATEGORY_INVENTORY_MKUP"_J:
			case "CATEGORY_INVENTORY_HAIR"_J:
			case "CATEGORY_INVENTORY_EYEBROWS"_J:
			case "CATEGORY_INVENTORY_CHEST_HAIR"_J:
			case "CATEGORY_INVENTORY_CONTACTS"_J:
			case "CATEGORY_INVENTORY_FACEPAINT"_J:
			case "CATEGORY_INVENTORY_BLUSHER"_J:
			case "CATEGORY_INVENTORY_LIPSTICK"_J:
			case "CATEGORY_INVENTORY_PROPERTY_INTERIOR"_J:
			case "CATEGORY_INVENTORY_WAREHOUSE"_J:
			case "CATEGORY_INVENTORY_CONTRABAND_MISSION"_J:
			case "CATEGORY_CONTRABAND_MISSION"_J:
			case "CATEGORY_CONTRABAND_QNTY"_J:
			case "CATEGORY_INVENTORY_WAREHOUSE_INTERIOR"_J:
			case "CATEGORY_WAREHOUSE_VEHICLE_INDEX"_J:
				return true;
			}

			return false;
		}

		bool HasGTAPlusMembership()
		{
			if (!NETWORK::NETWORK_HAS_SC_MEMBERSHIP_INFO())
				return false;

			struct SC_MEMBERSHIP_INFO
			{
				SCR_BOOL HasMembership;
				SCR_INT StartTime;
				SCR_INT EndTime;
			};

			SC_MEMBERSHIP_INFO info{};
			NETWORK::NETWORK_GET_SC_MEMBERSHIP_INFO(&info);

			return info.HasMembership;
		}

		int GetItemPrice(rage::netCatalogBaseItem* item)
		{
			if (item->m_MembershipPrice == -1 || !HasGTAPlusMembership())
				return item->m_Price;

			return item->m_MembershipPrice;
		}
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). Each item below specifies its own
	// width (Theme::kContentWidth) rather than the Grid itself, matching
	// Stand's real Grid - see Grid.hpp's class comment.
	TransactionsGrid::TransactionsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void TransactionsGrid::ResolveItem(ItemField& item)
	{
		item.hash = Joaat(item.name);
		if (auto* catItem = Pointers.GetCatalogItem(Pointers.NetCatalog, &item.hash))
		{
			item.isValid = true;
			item.intendedCategory = catItem->m_CategoryHash;
			item.intendedPrice = GetItemPrice(catItem);
		}
		else
		{
			item.isValid = false;
		}
	}

	// Mirrors EditTransactionItem()'s own validation half exactly (the
	// text-input half is AddItemFieldRows() below) - called fresh for
	// every item at the top of AddBasketRows()/AddServiceRows() on every
	// populate(), so it always reflects the current Category/Type even
	// when only *those* changed - see this class's own header comment.
	void TransactionsGrid::RevalidateItem(ItemField& item, bool required, bool validateCategory)
	{
		item.message.clear();
		item.messageIsError = false;

		const bool empty = item.name.empty() && !required;

		if (!item.isValid && !empty)
		{
			item.message = "Item not found!";
			item.messageIsError = true;
			return;
		}

		if (kBannedItemHashes.contains(item.hash))
		{
			item.message = "This item has been blocked for your safety";
			item.messageIsError = true;
			return;
		}

		if (validateCategory && !IsPriceModifier(item.intendedCategory) && item.intendedCategory != m_CategoryHash && !empty)
		{
			item.message = std::format("Item category {} does not match txn category {}", CategoryNameFromHash(item.intendedCategory), m_CategoryName);
			item.messageIsError = (m_Type == TxnType::Service);
		}
	}

	bool TransactionsGrid::ComputeTxnValid()
	{
		if (m_Type == TxnType::Basket)
		{
			for (auto& item : m_BasketItems)
				if (item.primary.messageIsError || item.secondary.messageIsError)
					return false;

			return true;
		}

		if (m_ServiceItem.messageIsError)
			return false;

		if (m_ServicePrice > m_ServiceItem.intendedPrice && m_ActionHash == "NET_SHOP_ACTION_EARN"_J)
			return false;

		return true;
	}

	// Mirrors OnTransactionTypeChanged() - except for one thing:
	// switching back to Basket in the original resets m_Category to
	// {"PROPERTY", "NET_SHOP_ACTION_BUY_PROPERTY"_J} - an *action* hash
	// used as a category hash, a copy-paste bug (the real category hash,
	// used everywhere else including this same struct's own default
	// member initializer, is "CATEGORY_PROPERTIE"_J). Fixed here rather
	// than reproduced.
	void TransactionsGrid::OnTypeChanged()
	{
		if (m_Type == TxnType::Service)
		{
			if (!IsCategoryService(m_CategoryHash))
			{
				m_CategoryName = "SERVICE";
				m_CategoryHash = "CATEGORY_SERVICE"_J;
			}
			if (!IsActionService(m_ActionHash))
			{
				m_ActionName = "EARN";
				m_ActionHash = "NET_SHOP_ACTION_EARN"_J;
			}
			m_BasketItems.clear();
		}
		else
		{
			if (IsCategoryService(m_CategoryHash))
			{
				m_CategoryName = "PROPERTY";
				m_CategoryHash = "CATEGORY_PROPERTIE"_J;
			}
			if (m_BasketItems.empty())
				m_BasketItems.push_back({});
		}
	}

	// Mirrors ProcessTransaction() exactly, called via one FiberPool job
	// (the original nests two - queueJob([]{ ProcessTransaction(info);
	// }) around ProcessTransaction's own queueJob([&info]{...}) - only
	// because `info` there is a function-local static captured by
	// reference across both; this snapshots everything into plain values
	// up front instead, so one job is enough).
	void TransactionsGrid::Trigger()
	{
		const auto type = m_Type;
		const auto categoryHash = m_CategoryHash;
		const auto actionHash = m_ActionHash;
		const auto basketItems = m_BasketItems;
		const auto serviceItem = m_ServiceItem;
		const auto servicePrice = m_ServicePrice;

		FiberPool::queueJob([type, categoryHash, actionHash, basketItems, serviceItem, servicePrice] {
			int txn_id = -1;
			bool txn_failed = false;
			CNetShopTransaction* txn = nullptr;

			Scripts::RunAsScript(Scripts::FindScriptThread("shop_controller"_J), [&] {
				if (type == TxnType::Basket)
				{
					if (NETSHOPPING::NET_GAMESERVER_BASKET_IS_ACTIVE())
						NETSHOPPING::NET_GAMESERVER_BASKET_END();

					if (!NETSHOPPING::NET_GAMESERVER_BASKET_START(&txn_id, categoryHash, actionHash, 4))
					{
						Notifications::Show("Transactions", "Failed to create basket", NotificationType::Error);
						txn_failed = true;
						NETSHOPPING::NET_GAMESERVER_BASKET_END();
						return;
					}

					struct NETSHOPPING_BASKET_ITEM
					{
						SCR_HASH PrimaryHash;
						SCR_HASH SecondaryHash;
						SCR_INT Value;
						SCR_INT StatValue;
					};
					static_assert(sizeof(NETSHOPPING_BASKET_ITEM) == 4 * 8);

					for (auto& item : basketItems)
					{
						NETSHOPPING_BASKET_ITEM scr_item{};
						scr_item.PrimaryHash = item.primary.hash;
						scr_item.SecondaryHash = item.secondary.hash;
						scr_item.Value = item.price;
						scr_item.StatValue = item.statValue;

						if (!NETSHOPPING::NET_GAMESERVER_BASKET_ADD_ITEM(&scr_item, item.quantity))
						{
							Notifications::Show("Transactions", std::format("Failed to add {} (x{}) to basket", item.primary.name, item.quantity), NotificationType::Error);
							txn_failed = true;
							NETSHOPPING::NET_GAMESERVER_BASKET_END();
							return;
						}
					}
				}
				else
				{
					if (!NETSHOPPING::NET_GAMESERVER_BEGIN_SERVICE(&txn_id, categoryHash, serviceItem.hash, actionHash, servicePrice, 4))
					{
						Notifications::Show("Transactions", "Failed to create service", NotificationType::Error);
						txn_failed = true;
						return;
					}
				}

				if (type == TxnType::Basket)
					txn = Pointers.GetActiveBasket(*Pointers.TransactionMgr, &txn_id);

				if (!NETSHOPPING::NET_GAMESERVER_CHECKOUT_START(txn_id))
				{
					Notifications::Show("Transactions", "Failed to begin checkout", NotificationType::Error);
					txn_failed = true;
					return;
				}
			});

			if (txn && txn->m_Running)
			{
				while (txn->m_Status == 0 || txn->m_Status == 1)
					Script::current()->yield();

				if (txn->m_Status == 3)
					Notifications::Show("Transactions", "Transaction complete", NotificationType::Success);
				else
					Notifications::Show("Transactions", "Transaction failed", NotificationType::Error);
			}
		});
	}

	// One text field (committing resolves the item via natives and, if
	// given, calls onResolved - e.g. copying the newly-resolved price
	// into the owning BasketItem/service price) plus, only when there's
	// something to say, its own validation-message row underneath.
	void TransactionsGrid::AddItemFieldRows(std::vector<std::unique_ptr<GridItem>>& items_draft, const std::string& label, ItemField& item, bool required, bool validateCategory, std::function<void()> onResolved)
	{
		items_draft.push_back(std::make_unique<GridItemTextInput>(Theme::kContentWidth,
		    kItemH,
		    label,
		    item.name,
		    [this, &item, onResolved](const std::string& value) {
			    item.name = value;
			    ResolveItem(item);
			    // Mirrors EditTransactionItem()'s own "modified" flag -
			    // only set (and only there acted on, copying the
			    // resolved price in) when resolution actually
			    // succeeded, so a failed re-resolve can't clobber
			    // price/statValue with a stale intendedPrice left over
			    // from a previous successful one.
			    if (onResolved && item.isValid)
				    onResolved();
			    invalidate();
		    }));

		if (!item.message.empty())
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, item.message, item.messageIsError ? Theme::kError : Theme::kText));
	}

	void TransactionsGrid::AddBasketRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Basket", Theme::kText));

		// Same field-swap ShouldSwapItemParams() drives in the original:
		// for inventory-style categories, the "Item" field is actually
		// secondary and the primary field is an "Inventory Slot" index;
		// every other category has it the other way around.
		const bool swapParams = ShouldSwapItemParams(m_CategoryHash);

		for (size_t i = 0; i < m_BasketItems.size(); i++)
		{
			auto& basketItem = m_BasketItems[i];

			if (swapParams)
			{
				RevalidateItem(basketItem.secondary, true, false);
				RevalidateItem(basketItem.primary, true, true);
				AddItemFieldRows(items_draft, std::format("Item [{}]", i), basketItem.secondary, true, false, [this, &basketItem] {
					basketItem.price = basketItem.secondary.intendedPrice;
				});
				AddItemFieldRows(items_draft, std::format("Inventory Slot [{}]", i), basketItem.primary, true, true, nullptr);
			}
			else
			{
				RevalidateItem(basketItem.primary, true, true);
				RevalidateItem(basketItem.secondary, false, false);
				AddItemFieldRows(items_draft, std::format("Item [{}]", i), basketItem.primary, true, true, [this, &basketItem] {
					basketItem.price = basketItem.primary.intendedPrice;
				});
				AddItemFieldRows(items_draft, std::format("Secondary Item [{}]", i), basketItem.secondary, false, false, nullptr);
			}

			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    std::format("Quantity [{}]", i),
			    static_cast<int>(basketItem.quantity),
			    0,
			    999,
			    [this, i](int value) {
				    if (i >= m_BasketItems.size())
					    return;

				    // Assume the user wants this item gone, same
				    // shortcut the original's own Quantity field has.
				    if (value == 0)
					    m_BasketItems.erase(m_BasketItems.begin() + i);
				    else
					    m_BasketItems[i].quantity = static_cast<std::uint32_t>(value);

				    invalidate();
			    }));

			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    std::format("Price [{}]", i),
			    basketItem.price,
			    std::numeric_limits<int>::min(),
			    std::numeric_limits<int>::max(),
			    [this, i](int value) {
				    if (i < m_BasketItems.size())
					    m_BasketItems[i].price = value;
			    }));

			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    std::format("Stat Value [{}]", i),
			    basketItem.statValue,
			    std::numeric_limits<int>::min(),
			    std::numeric_limits<int>::max(),
			    [this, i](int value) {
				    if (i < m_BasketItems.size())
					    m_BasketItems[i].statValue = value;
			    }));

			if (m_BasketItems.size() > 1)
			{
				items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, std::format("Delete [{}]", i), [this, i] {
					if (i < m_BasketItems.size())
						m_BasketItems.erase(m_BasketItems.begin() + i);
					invalidate();
				}));
			}
		}

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Add Item", [this] {
			m_BasketItems.push_back({});
			invalidate();
		}));
	}

	void TransactionsGrid::AddServiceRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Service", Theme::kText));

		RevalidateItem(m_ServiceItem, true, true);
		AddItemFieldRows(items_draft, "Item", m_ServiceItem, true, true, [this] {
			m_ServicePrice = m_ServiceItem.intendedPrice;
		});

		if (m_ServiceItem.intendedPrice != 0 || m_ActionHash != "NET_SHOP_ACTION_EARN"_J)
		{
			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    "Price",
			    m_ServicePrice,
			    std::numeric_limits<int>::min(),
			    std::numeric_limits<int>::max(),
			    [this](int value) {
				    m_ServicePrice = value;
				    invalidate(); // refreshes the "exceeds maximum" warning below
			    }));

			if (m_ServicePrice > m_ServiceItem.intendedPrice && m_ActionHash == "NET_SHOP_ACTION_EARN"_J)
				items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Item price exceeds maximum allowed ({})", m_ServiceItem.intendedPrice), Theme::kError));
		}
	}

	void TransactionsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		if (!watchCondition([] {
			    return NativeInvoker::AreHandlersCached();
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Natives not cached yet", Theme::kError));
			return;
		}

		if (watchCondition([] {
			    return AnticheatBypass::IsFSLProvidingLocalSaves();
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Transactions are not supported with FSL local saves enabled", Theme::kError));
			return;
		}

		if (!watchCondition([] {
			    return NETSHOPPING::NET_GAMESERVER_CATALOG_IS_VALID();
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Catalog not loaded yet", Theme::kError));
			return;
		}

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth,
		    kItemH,
		    "Warning: You are solely responsible for what you do with this tool. If you don't know what you're doing, you'll likely get banned",
		    Theme::kError));

		items_draft.push_back(std::make_unique<GridItemLocalList>(Theme::kContentWidth,
		    kItemH,
		    "Type",
		    std::vector<std::string>{"Basket", "Service"},
		    static_cast<int>(m_Type),
		    [this](int index) {
			    m_Type = static_cast<TxnType>(index);
			    OnTypeChanged();
			    invalidate();
		    }));

		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [this] {
			    std::vector<std::string> rows;
			    for (auto& [name, hash] : kNetShopCategories)
				    if ((m_Type != TxnType::Service) != IsCategoryService(hash))
					    rows.push_back(name);
			    return rows;
		    },
		    [this](size_t index, const std::string&, bool, bool, bool) {
			    std::vector<std::pair<const char*, std::uint32_t>> filtered;
			    for (auto& entry : kNetShopCategories)
				    if ((m_Type != TxnType::Service) != IsCategoryService(entry.second))
					    filtered.push_back(entry);

			    if (index >= filtered.size())
				    return;

			    m_CategoryName = filtered[index].first;
			    m_CategoryHash = filtered[index].second;
			    invalidate();
		    }));

		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [this] {
			    std::vector<std::string> rows;
			    for (auto& [name, hash] : kNetShopActions)
				    if (m_Type != TxnType::Service || IsActionService(hash))
					    rows.push_back(name);
			    return rows;
		    },
		    [this](size_t index, const std::string&, bool, bool, bool) {
			    std::vector<std::pair<const char*, std::uint32_t>> filtered;
			    for (auto& entry : kNetShopActions)
				    if (m_Type != TxnType::Service || IsActionService(entry.second))
					    filtered.push_back(entry);

			    if (index >= filtered.size())
				    return;

			    m_ActionName = filtered[index].first;
			    m_ActionHash = filtered[index].second;
			    invalidate();
		    }));

		if (m_Type == TxnType::Basket)
			AddBasketRows(items_draft);
		else
			AddServiceRows(items_draft);

		if (ComputeTxnValid())
		{
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Trigger", [this] {
				Trigger();
			}));
		}
		else
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "The transaction isn't valid. Ensure that all fields are filled out correctly", Theme::kError));
		}
	}
}
