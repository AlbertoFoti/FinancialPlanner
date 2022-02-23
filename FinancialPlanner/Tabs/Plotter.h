#pragma once

#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"
#include <math.h>

#define PLOT_PADDING_UNIX_TIME 2592000
#define PLOT_PADDING_Y 50   // To do : dynamic padding based on min and max value of input data

class Plotter {
public: 
	// Line Plots
	void ShowLinePlot_def(const char* label_id, const double* xs, const double* ys, int count);
	
	// Candle Plots
	void ShowCandleBarsPlot(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, float width_percent, ImVec4 bullCol, ImVec4 bearCol, double x_from, double x_to, double y_min, double y_max);
	void ShowCandleBarsPlot_default(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count);
private:
	void CandleBarsPlot(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, float width_percent, ImVec4 bullCol, ImVec4 bearCol);
};