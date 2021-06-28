"""
Purpose: format station file for QuakeMigrate from IRIS GMAP
         (https://ds.iris.edu/gmap/) output.

         QuakeMigrate station file format is:
         Latitude, Longitude, Elevation (km), Station Name
"""

# define the IRIS GMAP station file path
gmap_Station_File = "stations.txt"
# define path that QuakeMigrate station file will be written to
qm_Station_File = "qm_stations.txt"

# initialize dict to store station information, and a list for networks
station_Dict = {}
networks = []

# read the gmap station file line by line
with open(gmap_Station_File, 'r') as file:
    for index, line_Contents in enumerate(file):

        # skip first three header lines
        if index < 3:
            continue
        else:
            split_Line = line_Contents.split("|")
            # keep track of network codes to download response files
            if split_Line[0] not in networks:
                networks.append(split_Line[0])
            # store station information in dict with station name as key
            station_Dict[split_Line[1]] = [split_Line[2], split_Line[3],
                                           float(split_Line[4]) / 1000]

print(f"Networks: {networks}")

# write the new QuakeMigrate station file line by line
with open(qm_Station_File, 'w') as file:
    # write header
    file.write("Latitude,Longitude,Elevation,Name\n")

    # loop through all stations in station dict
    for key in station_Dict.keys():

        # line format: Latitude, Longitude, Elevation (km), Station Name
        line = ""

        # convert all line items to string
        for item in station_Dict[key]:
            line += str(item) + ","
        # add station name to line
        line += key
        # add newline to line
        line += "\n"

        # write line to file
        file.write(line)
