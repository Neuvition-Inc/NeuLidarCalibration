#pragma once

#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/features/integral_image_normal.h>  //���߹�����ͷ�ļ�
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>

//icp
#include <pcl/registration/icp.h>

//��ת
#include <pcl/common/transforms.h>
#include <pcl/common/distances.h>
//���ģ��
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_circle3d.h>
#include <pcl/sample_consensus/sac_model_circle.h>
#include <pcl/sample_consensus/sac_model_plane.h>

#include <pcl/sample_consensus/sac_model_line.h>

//ѹ��
#include <pcl/compression/octree_pointcloud_compression.h>

//����
#include <pcl/segmentation/extract_clusters.h>

//Eigen
#include <Eigen/Core>
#include <Eigen/Dense>

//
#include <pcl/filters/voxel_grid.h>

//����

#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/radius_outlier_removal.h>

//��Ե�ɼ�
#include <pcl/features/boundary.h>
//����
#include <pcl/kdtree/kdtree.h>

//����
#include <pcl/filters/statistical_outlier_removal.h>
//������

#include <pcl/keypoints/harris_3d.h>

//ͶӰ�˲�
#include <pcl/filters/project_inliers.h> 

//������
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

