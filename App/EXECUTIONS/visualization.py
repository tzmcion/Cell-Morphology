import os
import pandas as pd
import numpy as np
from sklearn.neighbors import KernelDensity
from bokeh.models import ColumnDataSource, Label, PrintfTickFormatter, Slider
from bokeh.palettes import Dark2_5 as colors
from bokeh.plotting import figure, show, save, output_file
from bokeh.layouts import column

FOLDER = os.getcwd()

PATH_TO_VISUALIZE = FOLDER + "/OUT/A01"

#Path from that folder to csv for classification
CLASSIFICATION_CSV = PATH_TO_VISUALIZE + "/CLASSIFICATION/DATA_WATERSHED.csv"
#path to count
COUNT_CSV = PATH_TO_VISUALIZE + "/Fast_Execution/COUNT_data.csv"
#path for surface
SURFACE_CSV = PATH_TO_VISUALIZE + "/Fast_Execution/SURFACE_data.csv"
#and path for tracking
TRACKING_CSVS = PATH_TO_VISUALIZE + "/Fast_Execution/TRACKING" #This ones need to be mapped

for s_path in os.scandir(FOLDER + "/OUT/"):
    print(s_path.path)
    #Path from that folder to csv for classification
    CLASSIFICATION_CSV = s_path.path + "/CLASSIFICATION/DATA_WATERSHED.csv"
    #path to count
    COUNT_CSV = s_path.path + "/Fast_Execution/COUNT_data.csv"
    #path for surface
    SURFACE_CSV = s_path.path + "/Fast_Execution/SURFACE_data.csv"
    #and path for tracking
    TRACKING_CSVS = s_path.path + "/Fast_Execution/TRACKING" #This ones need to be mapped

    #logic:

    #TRACKING::

    def get_all_tracking(path,keyword="merged_", ignore_first=True):
        all_paths = []
        first = True
        for file in os.scandir(path):
            path = file.path
            if(path.find(keyword) != -1):
                if(ignore_first):
                    if(first):
                        first=False
                        continue
                all_paths.append(path)
        return all_paths

    def create_dataframe_from_path(path:str):
        df = pd.read_csv(path, names=["Radius","Value"], sep=';')
        df["Value"] = pd.to_numeric(df["Value"])
        total = df["Value"].sum()
        df["Value"] = np.floor((df["Value"] / total) * 500).astype(np.int32)
        return df
        
    dataframes = []
    first = 1
    second = 4
    for path in get_all_tracking(TRACKING_CSVS):
        name = "Sample: " + str(first) + " to " + str(second) + ", SUM: " + str(create_dataframe_from_path(path)["Value"].sum())
        first += 4
        second += 4
        dataframes.append({
            "Name":name,
            "Data":create_dataframe_from_path(path)
        })

    MIN = dataframes[0]["Data"].loc[0].at['Radius']
    MAX = (dataframes[0]["Data"].loc[1].at['Radius'] - MIN)*45

    x = np.linspace(MIN-4,MAX+4,1000)
    source = ColumnDataSource(dict(x=x))

    p = figure(title="Cell distance from each other histogram", height=800,width=1400, x_range=(MIN-4, MAX+4), x_axis_label="Distance from another cell", y_axis_label="Percentage of cells")

    for group_df, color in zip(dataframes, colors):
        name = group_df["Name"]
        df_vals = group_df["Data"]
        radiuses = df_vals["Radius"].values
        counts = df_vals['Value'].values
        data = np.repeat(radiuses,counts.astype(int))
        kde = KernelDensity(kernel="gaussian", bandwidth=MAX/90).fit(data[:, np.newaxis])
        log_density = kde.score_samples(x[:, np.newaxis])
        y = np.exp(log_density)
        y = y / y.sum() * 100
        source.add(y, name)
        p.varea(x="x", y1=name, y2=0, source=source, fill_alpha=0.5, fill_color=color, legend_label=name)

        # Find the highest point and annotate with a label
        max_idx = np.argmax(y)
        highest_point_label = Label(
            x=x[max_idx],
            y=y[max_idx],
            text=name,
            text_font_size="10pt",
            x_offset=10,
            y_offset=-2,
            text_color=color,
        )
        p.add_layout(highest_point_label)

    # Display x-axis labels as percentages
    p.xaxis.formatter = PrintfTickFormatter()
    p.yaxis.formatter = PrintfTickFormatter(format="%d%%")
    p.x_range.start = 4

    p.axis.axis_line_color = None
    p.axis.major_tick_line_color = None
    p.axis.minor_tick_line_color = None

    p.xgrid.grid_line_color = None
    p.xaxis.ticker = np.round(np.linspace(MIN - 4,MAX + 4,20),1)
    p.yaxis.ticker = list(range(0, 101, 1))
    p.y_range.start = 0
    p.legend.click_policy = "hide"
    # show(p)

    ##
    ##COUNT and CLASSIFICATION::
    ##

    count_df = pd.read_csv(COUNT_CSV, names=["Sample","Quantity","None"], sep=';')
    x_axis = np.linspace(0,len(count_df.index),len(count_df.index),dtype=np.uint8)
    y_axis = count_df["Quantity"].to_numpy()
    c = figure(title="Quantity of cells", x_axis_label="Sample", y_axis_label="Quantity of cells", width=800)
    c.line(x_axis,y_axis,legend_label="Total Count", line_width=2, color=(255,0,255))
    if(os.path.exists(CLASSIFICATION_CSV)):
        classification_df = pd.read_csv(CLASSIFICATION_CSV, names=["Sample", "Mean_surface", "Mean_SD", "Blob", "Round", "Not Resolvable"], sep=';')
        y_axis_blob = classification_df["Blob"].to_numpy()
        y_axis_round = classification_df["Round"].to_numpy()
        y_axis_unknown = classification_df["Not Resolvable"].to_numpy()
        c.line(x_axis,y_axis_blob,legend_label="Nr od Blob cells", line_width=2, color=(255,0,0))
        c.line(x_axis,y_axis_round,legend_label="Nr of Round cells", line_width=2, color=(0,0,255))
        c.line(x_axis,y_axis_unknown,legend_label="Imprecision Rate", line_width=2, color=(125,125,125))
    # show(c)


    ##
    ##SURFACE:
    ##

    surface_df = pd.read_csv(SURFACE_CSV,names=["Sample","Occupation_percent", "None"], sep=';')
    x_axis = np.linspace(0,len(surface_df.index),len(surface_df.index),dtype=np.uint8)
    y_axis = surface_df["Occupation_percent"].to_numpy()
    a = figure(title="Space occupied by cells", x_axis_label="Sample", y_axis_label="Percentage of image interpreted as cells", width=800, y_range=(0,100))
    a.line(x_axis,y_axis,legend_label="% of image occupied by cells", line_width=2, color=(255,20,20))
    # show(a)

    ##
    ##CLASSIFICATION:
    ##

    OUT_FILE = s_path.path + "/results.html"

    output_file(filename=OUT_FILE, title="Static HTML file")
    save(column(p, c, a))
