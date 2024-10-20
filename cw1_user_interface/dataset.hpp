// COMP2811 Coursework 1: QuakeDataset class

#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include "quake.hpp"
#include "csv.hpp"
#include <string>

using namespace csv;

class QuakeDataset
{
  public:
    QuakeDataset(): QuakeDataset("test0.csv") {}
  //inlined constructor to include filename
    QuakeDataset(const std::string&);

    void loadData(const std:: string& filename){
      CSVReader reader(filename);
      CSVRow row;    
      
      for (auto& rowi: reader){
        std::string time = rowi["time"].get<>();
        double lat = rowi["latitude"].get<double>();
        double lon = rowi["longitude"].get<double>();
        double dep = rowi["depth"].get<double>();
        double mag = rowi["mag"].get<double>();

        data.emplace_back(time,lat,lon,dep,mag);
      }

    }
    //retrun size of quake vector as integer
    int size(){
      std::size_t data_size = data.size();
      return static_cast<int>(data_size);
    }

    const Quake& operator [](std::size_t index) const{
      if(index >= data.size()){
        throw std::out_of_range("Index out of range");
      }
      return data[index];
    }
    //return highest magnitude earthquake data 
    const Quake strongest(){
      if (data.empty()){
        throw std::out_of_range("Data is empty");
      }
      double max=0;
      int current;
      for(std::size_t i=0; i<data.size(); i++){
        double temp =data[i].getMagnitude();
        if (temp > max){
          max=temp;
          current = i;
        }
      }
      return data[current];
    }
    //return lowest magnitude earthquake data
    const Quake shallowest(){
      if (data.empty()){
        throw std::out_of_range("Data is empty");
      }
      double min=10000.0;
      int current;
      for(std::size_t i=0; i<data.size();i++){
        double temp = data[i].getDepth();
        if(temp < min){
          min=temp;
          current=i;
        }
      }
      return data[current];
    }
    //return mean depth of quakes in dataset
    const double meanDepth(){
      if (data.empty()){
        throw std::out_of_range("Data is empty");
      }
      double total;
      for(std::size_t i=0; i<data.size();i++){
        double temp = data[i].getDepth();
        total+=temp;
      }
      double mean = total/size();
      return mean;
    }
    //return mean magnitude of quakes in dataset
    const double meanMagnitude(){
      if (data.empty()){
        throw std::out_of_range("Data is empty");
      }
      double total;
      for(std::size_t i=0; i<data.size();i++){
        double temp = data[i].getMagnitude();
        total+=temp;
      }
      double mean = total/size();
      return mean;
    }


  


    // Specify prototypes or inlined methods here
    // (see UML diagram for what is required)

  private:
    std::vector<Quake> data;
};
