#include "Commands/CommandToggleLegacy.hpp"
#include "Scripting/NativeHooks.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	static void NetworkCasinoCanBetHook(rage::scrNativeCallContext* ctx);
	static void NetworkCasinoCanBuyChipsPVCHook(rage::scrNativeCallContext* ctx);

	class BypassCasinoRestrictions : public CommandToggleLegacy
	{
		using CommandToggleLegacy::CommandToggleLegacy;

		virtual void OnEnable() override
		{
			static auto hooked = []() {
				NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::NETWORK_CASINO_CAN_BET, &NetworkCasinoCanBetHook);
				NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::NETWORK_CASINO_CAN_BUY_CHIPS_PVC, &NetworkCasinoCanBuyChipsPVCHook);
				return true;
			}();
		}
	};

	static BypassCasinoRestrictions _BypassCasinoRestrictions{"bypasscasinogeoblock", "Bypass Casino Restrictions", "Bypasses all casino region restrictions"}; // TODO: enable by default?

	static void NetworkCasinoCanBetHook(rage::scrNativeCallContext* ctx)
	{
		if (_BypassCasinoRestrictions.GetState())
			return ctx->SetReturnValue(TRUE);
		NativeInvoker::GetNativeHandler(NativeIndex::NETWORK_CASINO_CAN_BET)(ctx);
	}

	static void NetworkCasinoCanBuyChipsPVCHook(rage::scrNativeCallContext* ctx)
	{
		if (_BypassCasinoRestrictions.GetState())
			return ctx->SetReturnValue(TRUE);
		NativeInvoker::GetNativeHandler(NativeIndex::NETWORK_CASINO_CAN_BUY_CHIPS_PVC)(ctx);
	}
}