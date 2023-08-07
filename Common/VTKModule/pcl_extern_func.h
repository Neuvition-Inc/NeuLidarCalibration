#pragma once

#include <pcl/visualization/pcl_visualizer.h>
#include <iostream>
#include <vtkPlaneSource.h>   


//对应pcl中的bool pcl::visualization::PCLVisualizer::addPlane (const pcl::ModelCoefficients &coefficients, const std::string &id, int viewport)   
vtkSmartPointer<vtkPolyData> createPlane(const pcl::ModelCoefficients& coefficients, float scale[2] = nullptr);


vtkSmartPointer<vtkPolyData> createPlane(const pcl::ModelCoefficients &coefficients, double x, double y, double z, float scale[2] = nullptr);







