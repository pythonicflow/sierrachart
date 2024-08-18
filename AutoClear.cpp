#include "sierrachart.h"

/*
    This study automatically clears various market data (Recent Bid/Ask Volume, Current Traded Volume 
    and Market Depth Pulling/Stacking Data) when a new bar starts, based on user-configured options.
    Modified by pythonicflow based on the original gcUserStudies script. (thanks to Chad)
*/

// Get DateTime independent of replay
SCDateTime GetNow(SCStudyInterfaceRef sc)
{
	if (sc.IsReplayRunning())
		return sc.CurrentDateTimeForReplay;
	else
		return sc.CurrentSystemDateTime;
}

SCDLLName("AutoClear")
const SCString ContactInformation = "AutoClear study";

SCSFExport AutoClear(SCStudyInterfaceRef sc)
{
	int InputIndex = 0;
	
	// Inputs for clearing data when a new bar starts
	SCInputRef Input_ClearOnNewBar = sc.Input[InputIndex++]; // Clear on new bar
	SCInputRef Input_ClearRecentBidAskVolumeOnNewBar = sc.Input[InputIndex++]; // Clear Recent Bid/Ask Volume
	SCInputRef Input_ClearCurrentTradedBidAskVolumeOnNewBar = sc.Input[InputIndex++]; // Clear Current Traded Volume
	SCInputRef Input_ClearMarketDepthPullingStackingDataOnNewBar = sc.Input[InputIndex++]; // Clear Market Depth Pulling/Stacking Data

	if (sc.SetDefaults)
	{
		// Set the configuration and defaults
		sc.GraphName = "Auto Clear on New Bar";
		sc.AutoLoop = 0;
		sc.GraphRegion = 0;
		sc.UpdateAlways = 1;

		// Option to clear data when a new bar starts
		Input_ClearOnNewBar.Name = "Clear on New Bar";
		Input_ClearOnNewBar.SetDescription("Clear the data on each new bar");
		Input_ClearOnNewBar.SetYesNo(0);

		// Option to clear Recent Bid/Ask Volume when a new bar starts
		Input_ClearRecentBidAskVolumeOnNewBar.Name = "Clear Recent Bid/Ask Volume on New Bar";
		Input_ClearRecentBidAskVolumeOnNewBar.SetDescription("Clear Recent Bid/Ask Volume on each new bar");
		Input_ClearRecentBidAskVolumeOnNewBar.SetYesNo(0);

		// Option to clear Current Traded Volume when a new bar starts
		Input_ClearCurrentTradedBidAskVolumeOnNewBar.Name = "Clear Current Traded Volume on New Bar";
		Input_ClearCurrentTradedBidAskVolumeOnNewBar.SetDescription("Clear Current Traded Volume on each new bar");
		Input_ClearCurrentTradedBidAskVolumeOnNewBar.SetYesNo(0);

		// Option to clear Market Depth Pulling/Stacking Data when a new bar starts
		Input_ClearMarketDepthPullingStackingDataOnNewBar.Name = "Clear Market Depth Pulling/Stacking Data on New Bar";
		Input_ClearMarketDepthPullingStackingDataOnNewBar.SetDescription("Clear Market Depth Pulling/Stacking Data on each new bar");
		Input_ClearMarketDepthPullingStackingDataOnNewBar.SetYesNo(0);

		return;
	}

	// Do not clear data during a full recalculation
	if (sc.IsFullRecalculation)
		return;

	// Clear data when a new bar starts
	if (Input_ClearOnNewBar.GetYesNo() && sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED)
	{
		if (Input_ClearRecentBidAskVolumeOnNewBar.GetYesNo())
			sc.ClearRecentBidAskVolume();

		if (Input_ClearCurrentTradedBidAskVolumeOnNewBar.GetYesNo())
			sc.ClearCurrentTradedBidAskVolume();

		if (Input_ClearMarketDepthPullingStackingDataOnNewBar.GetYesNo())
			sc.ClearMarketDepthPullingStackingData();
	}
}
