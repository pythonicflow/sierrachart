#include "sierrachart.h"

/*
    PullStackDiff study for Sierra Chart.
    Displays cumulative pulling/stacking values and net difference within a specified tick range.

    @version 1.0
    @author pythonicflow (thanks to Luda)
*/

SCDLLName("PullStackDiff")

SCSFExport scsf_PullStackDiff(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.GraphName = "PullStackDiff";
        sc.StudyDescription = "Displays cumulative pulling/stacking values and net difference within a specified tick range.";
        sc.AutoLoop = 1;

        // Max Tick Range setting
        sc.Input[0].Name = "Max Tick Range";
        sc.Input[0].SetInt(24); // 24 ticks above and below the price

        // Subgraph setup with default colors
        sc.Subgraph[0].Name = "Cumulative ASK Pulling/Stacking";
        sc.Subgraph[0].DrawStyle = DRAWSTYLE_LINE;
        sc.Subgraph[0].PrimaryColor = RGB(255, 43, 97); // Default color for ASK
        sc.Subgraph[0].LineWidth = 2;

        sc.Subgraph[1].Name = "Cumulative BID Pulling/Stacking";
        sc.Subgraph[1].DrawStyle = DRAWSTYLE_LINE;
        sc.Subgraph[1].PrimaryColor = RGB(59, 191, 255); // Default color for BID
        sc.Subgraph[1].LineWidth = 2;

        sc.Subgraph[2].Name = "Net Pulling/Stacking Difference";
        sc.Subgraph[2].DrawStyle = DRAWSTYLE_BAR;
        sc.Subgraph[2].LineWidth = 1;
        sc.Subgraph[2].AutoColoring = AUTOCOLOR_POSNEG; // Automatic coloring for positive/negative values
        sc.Subgraph[2].PrimaryColor = RGB(59, 191, 255); // Positive color
        sc.Subgraph[2].SecondaryColor = RGB(255, 43, 97); // Negative color

        return;
    }

    // Process only the last bar in the dataset
    if (sc.Index != sc.ArraySize - 1)
        return;

    int MaxTickRange = sc.Input[0].GetInt();
    int CurrentPriceInTicks = sc.PriceValueToTicks(sc.LastTradePrice);
    int LowestPriceInTicks = CurrentPriceInTicks - MaxTickRange;
    int HighestPriceInTicks = CurrentPriceInTicks + MaxTickRange; 

    int Cumulative_BID_PullingStacking = 0;
    int Cumulative_ASK_PullingStacking = 0;

    // Accumulate pulling/stacking values across the specified price range
    for (int i = LowestPriceInTicks; i <= HighestPriceInTicks; ++i)
    {
        float PriceCheck = sc.TicksToPriceValue(i);
        Cumulative_ASK_PullingStacking += sc.GetAskMarketDepthStackPullValueAtPrice(PriceCheck);
        Cumulative_BID_PullingStacking += sc.GetBidMarketDepthStackPullValueAtPrice(PriceCheck);
    }

    // Calculate the net difference between BID and ASK pulling/stacking
    int NetDifference = Cumulative_BID_PullingStacking - Cumulative_ASK_PullingStacking;
    sc.Subgraph[0][sc.Index] = static_cast<float>(Cumulative_ASK_PullingStacking);
    sc.Subgraph[1][sc.Index] = static_cast<float>(Cumulative_BID_PullingStacking);
    sc.Subgraph[2][sc.Index] = static_cast<float>(NetDifference);

    // Ensure the study is updated on every tick
    sc.UpdateAlways = 1;
}