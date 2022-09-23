#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"
#include <math.h>

/**
 * @brief Plot left-rifht padding on the x-axis (in seconds)
 * 
 */
#define PLOT_PADDING_UNIX_TIME 2592000

/**
 * @brief Padding as a percentage of the y-variable total swing
 * 
 */
#define PLOT_PADDING_Y_PERC 0.05

/**
 * @brief 
 * 
 */
#define PLOT_PERC_OF_SCREEN_Y 0.30

/**
 * @brief Plotter Class to help in building plots
 * 
 */
class Plotter {
public: 
	/**
	 * @brief Empty Plot
	 * 
	 * @param label_id unique id of the plot
	 */
	static void ShowEmptyPlot(const char* label_id);

	/**
	 * @brief Line Plot (default)
	 * 
	 * @param label_id self-explanatory
	 * @param xs x-axis values vector (in UNIX time)
	 * @param ys y-axis values vector 
	 * @param count number of data-points
	 */
	static void ShowLinePlot_def(const char* label_id, const double* xs, const double* ys, int count);
	
	/**
	 * @brief Candle Bar Plot (Custom)
	 * 
	 * @param label_id self-explanatory
	 * @param xs x-axis values vector (in UNIX time)
	 * @param opens y-axis opening values vector
	 * @param closes y-axis closing values vector
	 * @param lows y-axis low values vector
	 * @param highs y-axis high values vector
	 * @param count number of data-points
	 * @param width_percent candle bar width parameter
	 * @param bullCol Positive Candle Color
	 * @param bearCol Negative Candle Color
	 * @param x_from x-axis left limit (in UNIX time)
	 * @param x_to x-axis left limit (in UNIX time)
	 * @param y_min y-axis left limit
	 * @param y_max y-axis left limit
	 */
	static void ShowCandleBarsPlot(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, float width_percent, ImVec4 bullCol, ImVec4 bearCol, double x_from, double x_to, double y_min, double y_max);

	/**
	 * @brief Candle Bar Plot (Default)
	 * 
	 * @param label_id self-explanatory
	 * @param xs x-axis values vector (in UNIX time)
	 * @param opens y-axis opening values vector
	 * @param closes y-axis closing values vector
	 * @param lows y-axis low values vector
	 * @param highs y-axis high values vector
	 * @param count number of data-points
	 */
	static static void ShowCandleBarsPlot_default(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count);

	/**
	 * @brief Empty Bar Groups plot (for debug and GUI planning)
	 * 
	 * @param label_id self-explanatory
	 */
	static void ShowBarGroupsPlot_empty(const char* label_id);

	static void ShowBarGroupsPlot_default(const char* label_id, const double* data, int count);
private:
	/**
	 * @brief Primitive to build candle bar plots
	 * 
	 * @param label_id self-explanatory
	 * @param xs x-axis values vector (in UNIX time)
	 * @param opens y-axis opening values vector
	 * @param closes y-axis closing values vector
	 * @param lows y-axis low values vector
	 * @param highs y-axis high values vector
	 * @param count number of data-points
	 * @param width_percent candle bar width parameter
	 * @param bullCol Positive Candle Color
	 * @param bearCol Negative Candle Color
	 */
	static void CandleBarsPlot(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, float width_percent, ImVec4 bullCol, ImVec4 bearCol);
};