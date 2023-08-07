#pragma once

#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/features/integral_image_normal.h>  //法线估计类头文件
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>

//icp
#include <pcl/registration/icp.h>

//旋转
#include <pcl/common/transforms.h>
#include <pcl/common/distances.h>
//拟合模型
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_circle3d.h>
#include <pcl/sample_consensus/sac_model_circle.h>
#include <pcl/sample_consensus/sac_model_plane.h>

#include <pcl/sample_consensus/sac_model_line.h>

//压缩
#include <pcl/compression/octree_pointcloud_compression.h>

//聚类
#include <pcl/segmentation/extract_clusters.h>

//Eigen
#include <Eigen/Core>
#include <Eigen/Dense>

//
#include <pcl/filters/voxel_grid.h>

//采样

#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/radius_outlier_removal.h>

//边缘采集
#include <pcl/features/boundary.h>
//排序
#include <pcl/kdtree/kdtree.h>

//过滤
#include <pcl/filters/statistical_outlier_removal.h>
//特征点

#include <pcl/keypoints/harris_3d.h>

//投影滤波
#include <pcl/filters/project_inliers.h> 

//随机拟合
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/point_types.h>
#include <pcl/filters/crop_box.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/correspondence.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/features/shot_omp.h>
#include <pcl/features/board.h>
#include <pcl/keypoints/uniform_sampling.h>
#include <pcl/recognition/cg/hough_3d.h>
#include <pcl/recognition/cg/geometric_consistency.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/kdtree/kdtree_flann.h>
//#include <pcl/kdtree/impl/kdtree_flann.hpp>
#include <pcl/common/transforms.h>
#include <pcl/console/parse.h>
#include <pcl/common/intersections.h>

#include <pcl/common/angles.h>

#define  IN 
#define  OUT

