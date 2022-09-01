#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

typedef vector<int> vec;

class ACO{
    public:
        vec solution;
        double result = 1000000000.0;

        void excecution(string name, int ant, double alpha, double beta, double Q, double rho){
            
            srand((unsigned)time(NULL));
            initialization(name);

            for(int i = 0; i<ant; i++){
                construction(alpha, beta);
                local_search();
                update_phromone(Q, rho);
                determination();
            }

            // cout << "Best solution: " ;
            // for (auto i: solution){
            //     cout << i << " ";
            // }
            // cout << endl;
            cout << "Distance: " << result << endl;
            
                    
        }


    
    private:
        vector<vec> all_coordinate;
        vector<vector<double>> pheromone;
        vec all_city;
        vec unvisited_city;
        vec current_solution;


        vector<vec> read_data(string data_name){

            ifstream data_file;
            data_file.open(data_name);
            vec coordinate;  
            if (data_file.is_open()){
                int number;
                while(data_file >> number){
                    coordinate.push_back(number);
                }
            }

            vector<vec> data;
            vec a;
            for (int i = 0; i<(coordinate.size()/3); i++){
                a.clear();
                a.push_back(coordinate[i*3]);
                a.push_back(coordinate[i*3+1]);
                a.push_back(coordinate[i*3+2]);
                data.push_back(a);        
            }
                
            return data;
        }

        void initialization(string file_name){

            //讀檔案
            all_coordinate = read_data(file_name);

            //set all city vetor
            for (int i=0; i< all_coordinate.size(); i++){
                all_city.push_back(all_coordinate[i][0]);
            }

            //set pheromone
            vector<double> minor(all_coordinate.size(), 0.0001);
            for (int k=0; k<all_coordinate.size(); k++){
                pheromone.push_back(minor);
            }
        }


        double evaluation_distance(vec a, vec b){
            return sqrt(pow((a[1]-b[1]), 2)+pow((a[2]-b[2]), 2));
        }

        double total_distance(vec solution){
            double object_value = 0.0;
            for (int i = 0; i < solution.size()-1; i++){
                object_value += evaluation_distance(all_coordinate[solution[i]-1], all_coordinate[solution[i+1]-1]);
            }

            return object_value;
        }


        void selection_rw(double alpha, double beta){

            int current_point = current_solution[(current_solution.size())-1];
            vector<double> eta; //距離倒數
            double prob = 0.0;
            vector<double> probability;
            for (int i = 0; i < unvisited_city.size(); i++){
                int unvisited = unvisited_city[i];
                eta.push_back(1.0/(evaluation_distance(all_coordinate[current_point-1], all_coordinate[unvisited-1])));
                prob += pow(pheromone[current_point-1][unvisited-1], alpha) * pow(eta[i], beta);
            }

            for (int i = 0; i < unvisited_city.size(); i++){
                int unvisited = unvisited_city[i];
                double p = (pow(pheromone[current_point-1][unvisited-1], alpha) * pow(eta[i], beta)) / prob;
                probability.push_back(p);
            }

            //隨機生成數
            double val = (double)rand() / RAND_MAX;
            double temp = 0.0;
            int selection;
            for (int k = 0; k < probability.size(); k++){ 
                temp += probability[k];
                if(val<temp){
                    selection = unvisited_city[k];
                    k = probability.size();
                }
            } 

            current_solution.push_back(selection);
            unvisited_city.erase(remove(unvisited_city.begin(), unvisited_city.end(), selection), unvisited_city.end());

        }

        void construction(double alpha, double beta){  

            current_solution.clear();
            unvisited_city = all_city;
            int first = rand() % all_city.size();
            current_solution.push_back(all_city[first]);
            unvisited_city.erase(remove(unvisited_city.begin(), unvisited_city.end(), all_city[first]), unvisited_city.end());
            while (!unvisited_city.empty()){
                selection_rw(alpha, beta);
            }
            current_solution.push_back(current_solution[0]);    //回到原點
                    
        }

        void update_phromone(double Q, double rho){
            int size = current_solution.size();
            vector<vector<double>> delta;
            //初始值0
            vector<double> zero((size-1), 0.0);
            for (int k=0; k<(size-1); k++){
                delta.push_back(zero);
            }
                    
            double current_result = total_distance(current_solution);
            for (int i =0; i < (size-1); i++){
                int a = current_solution[i];
                int b = current_solution[i+1];
                delta[a-1][b-1] = Q/current_result;
            }

            for (int i = 0; i<size-1; i++){
                for (int k = 0; k <size-1; k++){
                    pheromone[i][k] = (1.0-rho)*pheromone[i][k] + delta[i][k];
                }
            }
        }

        void determination(){
            double current_result = total_distance(current_solution);
            if (current_result < result){
                solution = current_solution;
                result = current_result;
            }
        }


        void local_search(){
            int size = current_solution.size();
            int num_a = rand() % (size-3);
            int num_b = (rand() % (size-num_a-3)) + (num_a+2);
            int city_a = current_solution[num_a];
            int city_a_plus = current_solution[num_a+1];
            int city_b = current_solution[num_b];
            int city_b_plus = current_solution[num_b+1];
            double distance_origin = evaluation_distance(all_coordinate[city_a-1], all_coordinate[city_a_plus-1])+
                                evaluation_distance(all_coordinate[city_b-1], all_coordinate[city_b_plus-1]);
            double distance_new = evaluation_distance(all_coordinate[city_a-1], all_coordinate[city_b-1])+
                                evaluation_distance(all_coordinate[city_a_plus-1], all_coordinate[city_b_plus-1]);
            if(distance_new < distance_origin){
                current_solution[num_a+1] = city_b;
                current_solution[num_b] = city_a_plus;
            }

        }

};

