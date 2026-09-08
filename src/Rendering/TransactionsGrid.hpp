#pragma once
#include "Rendering/Grid.hpp"
#include "Util/Joaat.hpp"

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	// Content grid for Recovery > Transactions: mirrors World/
	// Transactions.cpp's BuildTransactionsMenu() - not a live viewer of
	// the account's actual transaction history (there isn't one here or
	// in the original), but a "Triggerer" form for constructing and
	// firing one user-authored NET_SHOP transaction against the game's
	// shop_controller script - a Type (Basket/Service), Category/Action
	// pickers, and either a dynamically-sized basket of items or a
	// single service item, then Trigger.
	//
	// The basket item list is the one part of this page whose row count
	// changes - exclusively through this page's own Add Item/Delete
	// buttons (nothing in the game adds/removes an entry on its own
	// while this is open, so - unlike GridItemPlayerList/
	// GridItemSelectList's own "re-read a live source every call"
	// approach - this is the same "vector of structs, invalidate() on
	// Add/Delete" pattern already used throughout this porting effort
	// (e.g. StartScriptGrid's Arg[i] rows, Outfit Editor's appendages).
	//
	// Per-item validation (item-not-found/banned/category-mismatch) is
	// the original's own EditTransactionItem() logic, recomputed for
	// every item at the top of populate() (so it reflects the current
	// Category/Type even when only *those* changed, not the item text
	// itself) and shown live via GridItemLiveText reading each item's
	// own stored message - not re-validated every frame the way the
	// original's own immediate-mode render does, but every actual state
	// change that could affect validity already goes through invalidate()
	// (a Category/Type change, or an item's own name being committed),
	// so it never goes stale in between.
	class TransactionsGrid : public Grid
	{
	public:
		TransactionsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		enum class TxnType
		{
			Basket,
			Service
		};

		// Mirrors TransactionItemHash - the original's own bare char[128]
		// buffer becomes a std::string; m_IsValid/m_IntendedCategory/
		// m_IntendedPrice are unchanged in spirit. message/messageIsError
		// are this port's own addition, replacing the original's inline
		// SetTransactionError()/SetTransactionWarning() ImGui calls (see
		// this class's own header comment on RevalidateItem()).
		struct ItemField
		{
			std::string name;
			std::uint32_t hash = 0;
			std::uint32_t intendedCategory = 0;
			int intendedPrice = 0;
			bool isValid = false;
			std::string message;
			bool messageIsError = false;
		};

		// Mirrors BasketItem.
		struct BasketItem
		{
			ItemField primary;
			ItemField secondary;
			std::uint32_t quantity = 1;
			int statValue = 1;
			int price = 0;
		};

		void ResolveItem(ItemField& item);
		void RevalidateItem(ItemField& item, bool required, bool validateCategory);
		bool ComputeTxnValid();
		void OnTypeChanged();
		void Trigger();

		void AddBasketRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddServiceRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddItemFieldRows(std::vector<std::unique_ptr<GridItem>>& items_draft, const std::string& label, ItemField& item, bool required, bool validateCategory, std::function<void()> onResolved);

		TxnType m_Type = TxnType::Basket;
		std::string m_CategoryName = "PROPERTY";
		std::uint32_t m_CategoryHash = Joaat("CATEGORY_PROPERTIE");
		std::string m_ActionName = "BUY_PROPERTY";
		std::uint32_t m_ActionHash = Joaat("NET_SHOP_ACTION_BUY_PROPERTY");

		std::vector<BasketItem> m_BasketItems{BasketItem{}};
		ItemField m_ServiceItem;
		int m_ServicePrice = 0;
	};
}
