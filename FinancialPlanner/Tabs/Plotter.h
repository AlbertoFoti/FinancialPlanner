#pragma once

#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"

#define PLOT_PADDING_UNIX_TIME 2592000

class Plotter {
public: 
	// Line Plots
	void ShowLinePlot_default(const char* label_id, const double* xs, const double* ys, int count) {
		if (ImPlot::BeginPlot(label_id)) {
			ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_Time, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit);
			//ImPlot::SetupAxesLimits(1570000000, 1640000000, 1, 30);
			ImPlot::PlotLine("Net Worth", xs, ys, count);
			ImPlot::EndPlot();
		}
	}
	
	// Candle Plots
	void ShowCandleBarsPlot(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, float width_percent, ImVec4 bullCol, ImVec4 bearCol, double x_from, double x_to, double y_min, double y_max);
	void ShowCandleBarsPlot_default(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count);
private:
	void CandleBarsPlot(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, float width_percent, ImVec4 bullCol, ImVec4 bearCol);
};