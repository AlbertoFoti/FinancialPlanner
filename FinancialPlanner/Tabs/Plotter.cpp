#include "Plotter.h"

void Plotter::ShowEmptyPlot(const char* label_id)
{
    double min_y = INFINITY;
    double max_y = -INFINITY;

    double xs[20] = { 0.0 };
    double ys[20] = { 0.0 };
    for (int i = 0; i != 20; ++i) {
        xs[i] = 0.0;
        ys[i] = 0.0;
    }

    if (ImPlot::BeginPlot(label_id, ImVec2(-1, 0))) {
        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_Time); //ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit);
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::PlotShaded(label_id, xs, ys, 20, 0);
        ImPlot::PopStyleVar();
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::PlotLine(label_id, xs, ys, 20);
        ImPlot::EndPlot();
    }
}

void Plotter::ShowLinePlot_def(const char* label_id, const double* xs, const double* ys, int count)
{
    float screenDim_y = ImGui::GetIO().DisplaySize.y;

    double min_y = INFINITY;
    double max_y = -INFINITY;
    for (int i = 0; i != count; ++i) {
        if (ys[i] > max_y) max_y = ys[i];
        if (ys[i] < min_y) min_y = ys[i];
    }

    if (ImPlot::BeginPlot(label_id, ImVec2(-1, screenDim_y * ((float)PLOT_PERC_OF_SCREEN_Y)))) {
        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_Time); //ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit);
        double delta = 0.0;
        double padding_y = 0.0;
        if ((max_y > 0 && min_y > 0) || (max_y < 0 && min_y < 0)) {
            delta = fabs(max_y) - fabs(min_y); // Same sign
        }
        else {
            delta = fabs(max_y) + fabs(min_y);// Different sign
        }
        padding_y = delta * PLOT_PADDING_Y_PERC;
        ImPlot::SetupAxesLimits(xs[0] - PLOT_PADDING_UNIX_TIME, xs[count - 1] + PLOT_PADDING_UNIX_TIME, min_y - padding_y, max_y + padding_y, ImGuiCond_None);
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::PlotShaded(label_id, xs, ys, count, 0);
        ImPlot::PopStyleVar();
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::PlotLine(label_id, xs, ys, count);
        ImPlot::EndPlot();
    }
}

void Plotter::ShowCandleBarsPlot(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, float width_percent, ImVec4 bullCol, ImVec4 bearCol, double x_from, double x_to, double y_min, double y_max)
{
    if (ImPlot::BeginPlot(label_id, ImVec2(-1, -1))) {
        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_Time); // ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit);
        ImPlot::SetupAxesLimits(x_from, x_to, y_min, y_max);
        ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.0f");
        this->CandleBarsPlot(label_id, xs, opens, closes, lows, highs, count, width_percent, bullCol, bearCol);
        ImPlot::EndPlot();
    }
}

void Plotter::ShowCandleBarsPlot_default(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count)
{
    static ImVec4 bullCol = ImVec4(0.000f, 1.000f, 0.441f, 1.000f);
    static ImVec4 bearCol = ImVec4(0.853f, 0.050f, 0.310f, 1.000f);

    double min_y = INFINITY;
    double max_y = -INFINITY;
    for (int i = 0; i != count; ++i) {
        if (highs[i] > max_y) max_y = highs[i];
        if (lows[i] < min_y) min_y = lows[i];
    }

    if (ImPlot::BeginPlot(label_id, ImVec2(-1, -1))) {
        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_Time);// ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit);
        double delta = 0.0;
        double padding_y = 0.0;
        if ((max_y > 0 && min_y > 0) || (max_y < 0 && min_y < 0)) {
            delta = fabs(max_y) - fabs(min_y); // Same sign
        }
        else {
            delta = fabs(max_y) + fabs(min_y);// Different sign
        }
        padding_y = delta * PLOT_PADDING_Y_PERC;
        ImPlot::SetupAxesLimits(xs[0] - PLOT_PADDING_UNIX_TIME, xs[count - 1] + PLOT_PADDING_UNIX_TIME, min_y - padding_y, max_y + padding_y);
        ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.0f");
        this->CandleBarsPlot(label_id, xs, opens, closes, lows, highs, count, 0.25f, bullCol, bearCol);
        ImPlot::EndPlot();
    }
}

void Plotter::ShowBarGroupsPlot_empty(const char* label_id)
{
    static ImS8  data[40] = {83, 67, 23, 89, 83, 78, 91, 82, 85, 90,  // Q1
                             80, 62, 56, 99, 55, 78, 88, 78, 90, 100, // Q2
                             80, 69, 52, 92, 72, 78, 75, 76, 89, 95,  // Q3
							 80, 69, 52, 92, 72, 78, 75, 76, 89, 95}; // Q4

	static const char*  ilabels[]   = {"Q1","Q2","Q3", "Q4"};
    static const char*  glabels[]   = {"2019","2020","2021","2022","2023","2024","2025","2026","2027","2028"};
    static const double positions[] = {0,1,2,3,4,5,6,7,8,9};

	// Deposits
	if (ImPlot::BeginPlot(label_id)) {
        ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
        ImPlot::SetupAxes("","",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
        ImPlot::SetupAxisTicks(ImAxis_X1,positions, 10, glabels);
        ImPlot::PlotBarGroups(ilabels,data,4,10,0.67f,0);
        ImPlot::EndPlot();
    }
}

void Plotter::CandleBarsPlot(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, float width_percent, ImVec4 bullCol, ImVec4 bearCol)
{
    // get ImGui window DrawList
    ImDrawList* draw_list = ImPlot::GetPlotDrawList();
    // calc real value width
    double half_width = count > 1 ? (xs[1] - xs[0]) * width_percent : width_percent;

    // begin plot item
    if (ImPlot::BeginItem(label_id)) {
        // override legend icon color
        ImPlot::GetCurrentItem()->Color = IM_COL32(64, 64, 64, 255);
        // fit data if requested
        if (ImPlot::FitThisFrame()) {
            for (int i = 0; i < count; ++i) {
                ImPlot::FitPoint(ImPlotPoint(xs[i], lows[i]));
                ImPlot::FitPoint(ImPlotPoint(xs[i], highs[i]));
            }
        }
        // render data
        for (int i = 0; i < count; ++i) {
            ImVec2 open_pos = ImPlot::PlotToPixels(xs[i] - half_width, opens[i]);
            ImVec2 close_pos = ImPlot::PlotToPixels(xs[i] + half_width, closes[i]);
            ImVec2 low_pos = ImPlot::PlotToPixels(xs[i], lows[i]);
            ImVec2 high_pos = ImPlot::PlotToPixels(xs[i], highs[i]);
            ImU32 color = ImGui::GetColorU32(opens[i] > closes[i] ? bearCol : bullCol);
            draw_list->AddLine(low_pos, high_pos, color);
            draw_list->AddRectFilled(open_pos, close_pos, color);
        }

        // end plot item
        ImPlot::EndItem();
    }
}
