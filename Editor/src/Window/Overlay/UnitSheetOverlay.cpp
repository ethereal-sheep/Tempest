#include "UnitSheetOverlay.h"

namespace Tempest
{
	void UnitSheetOverlay::open_popup(const Event& e)
	{
		open = true;
	}

	void UnitSheetOverlay::show(Instance& )
	{
		open = true;
	}
}