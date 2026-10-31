#include <cstddef>

static float(*s_override)[256] = nullptr;

extern "C" void rage_input_helper_set_override(float(*pArray)[256])
{
	s_override = pArray;
}

extern "C" float(*rage_input_helper_get_override())[256]
{
	return s_override;
}
