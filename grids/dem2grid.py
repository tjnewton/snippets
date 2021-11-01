# Script to sample a DEM on a grid of points using QGIS. This script should
# be run in the QGIS Python Console because at the time of creation the
# methods used below are not available via the QGIS API. This script is
# adapted from p.132 of "QGIS Python Programming Cookbook" by Joel Lawhead.

# QGIS -> Plugins -> Python Console
import numpy
from PyQt5.QtCore import *

# spacing between grid nodes in map coordinates
spacing = .001
#
rasterLyr = QgsRasterLayer("/qgis_data/rasters/satimage.tif", "Sat Image")
rasterLyr.isValid()
# True
rpr = rasterLyr.dataProvider()
epsg = rasterLyr.crs().postgisSrid()
ext = rasterLyr.extent()
vectorLyr = QgsVectorLayer('Point?crs=epsg:%s' % epsg, 'Grid' , "memory")
vpr = vectorLyr.dataProvider()
qd = QVariant.Double
vpr.addAttributes([QgsField("Red", qd), QgsField("Green", qd), QgsField("Blue", qd)])
vectorLyr.updateFields()
xmin = ext.xMinimum()
xmax = ext.xMaximum()
ymin = ext.yMinimum()
ymax = ext.yMaximum()

pts = [(x,y) for x in (i for i in numpy.arange(xmin, xmax, spacing)) for y in (j for j in numpy.arange(ymin, ymax, spacing))]
feats = []
for x,y in pts:
  f = QgsFeature()
  p = QgsPoint(x,y)
  qry = rasterLyr.dataProvider().identify(p, QgsRaster.IdentifyFormatValue)
  r = qry.results()
  f.setAttribute(0, r[1])
  f.setAttribute(1, r[2])
  f.setAttribute(2, r[3])
  f.setGeometry(QgsGeometry.fromPoint(p))
  feats.append(f)
vpr.addFeatures(feats)
vectorLyr.updateExtents()
QgsMapLayerRegistry.instance().addMapLayers([vectorLyr, rasterLyr])
canvas = qgis.utils.iface.mapCanvas()
canvas.setExtent(rasterLyr.extent())
canvas.refresh()

