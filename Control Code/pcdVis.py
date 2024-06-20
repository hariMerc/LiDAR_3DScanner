import open3d as o3d
import pcdWrite1 as pcd
pc_pcd = o3d.io.read_point_cloud('Scan20240613_1413.pcd')
o3d.visualization.draw_geometries([pc_pcd])
