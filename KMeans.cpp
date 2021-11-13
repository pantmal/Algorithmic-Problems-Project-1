#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm> 
#include <float.h>
#include <math.h>  

#include "KMeans.h"
#include "Cluster.h"
#include "VectorElement.h"


int findClosest(double arr[], int n, double target)
{
    // Corner cases
    if (target <= arr[0])
        return 0;
    if (target >= arr[n - 1])
        return n - 1;
 
    // Doing binary search
    int i = 0, j = n, mid = 0;
    while (i < j) {
        mid = (i + j) / 2;
 
        if (arr[mid] == target)
            return mid;
 
        /* If target is less than array element,
            then search in left */
        if (target < arr[mid]) {
 
            // If target is greater than previous
            // to mid, return closest of two
            if (mid > 0 && target > arr[mid - 1])
                return mid;
 
            /* Repeat for left half */
            j = mid;
        }
 
        // If target is greater than mid
        else {
            if (mid < n - 1 && target < arr[mid + 1])
                return mid+1;
            // update i
            i = mid + 1;
        }
    }
 
    // Only single element left after search
    return mid;
}


KMeans::KMeans(string assigner_arg, int clusters_arg){


    this->assigner = assigner_arg;
    this->clusters = clusters_arg;
}


void KMeans::initialization(VectorElement** Input_Array, int input_size){


    // pick first element random.

    int t_counter = 0;
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
    uniform_int_distribution<> U(0,input_size-1);

    VectorElement **init_centroids = new VectorElement *[clusters];
    this->ClusterArray = new Cluster *[clusters];
    
    int random_element = U(e);
    VectorElement* v_obg = Input_Array[random_element]; 
    init_centroids[t_counter] = v_obg;
    ClusterArray[t_counter] = new Cluster(init_centroids[t_counter],t_counter);

    
    while (t_counter != clusters-1){
        
        int p_size = input_size-(t_counter+1);
        double* p_array = new double[p_size];
        int p_counter = 0;
        
        VectorElement** centroid_candidates = new VectorElement*[p_size];
        
        double curr_sum = 0.0;

        double max_dist_all = -1.0;
        for (int i = 0; i < input_size; i++){
            bool is_centroid = false;
            for (int c = 0; c < (t_counter+1); c++){
                if (Input_Array[i]->id == init_centroids[c]->id){
                    is_centroid = true;
                }
            }

            if (is_centroid) continue;

            double min_dist = DBL_MAX;
            int min_id = -1;
            for (int k = 0; k < (t_counter+1); k++){

                VectorElement *vobj = Input_Array[i];
                vobj->getL2Distance(ClusterArray[k]->centroid);

                if (vobj->distanceCurrQ < min_dist){
                    min_dist = vobj->distanceCurrQ;
                    //min_id = ClusterArray[k]->id;
                } 
            }

            if (min_dist > max_dist_all){
                max_dist_all = min_dist;
                //max_dist_all = pow(min_dist,2);
            }

        }


        //cout << "max_dist_all " << max_dist_all <<endl;

        for (int i = 0; i < input_size; i++){
            bool is_centroid = false;
            for (int c = 0; c < (t_counter+1); c++){
                if (Input_Array[i]->id == init_centroids[c]->id){
                    is_centroid = true;
                }
            }

            if (is_centroid) continue;

            double min_dist = DBL_MAX;
            int min_id = -1;
            for (int k = 0; k < (t_counter+1); k++){

                VectorElement *vobj = Input_Array[i];
                vobj->getL2Distance(ClusterArray[k]->centroid);

                if (vobj->distanceCurrQ < min_dist){
                    min_dist = vobj->distanceCurrQ;
                    //min_id = ClusterArray[k]->id;
                } 
            }

            //cout << "dist is  " << min_dist << endl;

            min_dist = min_dist / max_dist_all;
            double dist = pow(min_dist,2);

            //dist = dist/min_dist;

            curr_sum += dist;
            p_array[p_counter] = curr_sum;
            centroid_candidates[p_counter] = Input_Array[i];
            p_counter++;

        }

        //double max_p = curr_sum/ p_array[p_size-1];

        //NORM
        // for (int p = 0; p < p_size;p++){
        //     p_array[p] = p_array[p] / p_array[p_size-1]; 
        // }

        // for (int p = 0; p < p_size;p++){
        //    myLogFile << p_array[p] << endl;
        // }

        //cout << "mp" << max_p <<" " <<curr_sum <<" " <<p_array[p_size-1] << endl;

        uniform_real_distribution<> Ux(0.0, curr_sum);

        double x = Ux(e);

        //cout << "x " <<x << endl;

        int index = findClosest(p_array,p_size,x);
        //cout << "index " << index << " " << p_array[index] << endl;
        
        t_counter++;
        VectorElement* v_obg = centroid_candidates[index]; 
        init_centroids[t_counter] = v_obg;
        ClusterArray[t_counter] = new Cluster(init_centroids[t_counter],t_counter);        


        delete[] centroid_candidates;
        delete[] p_array;
    }

    // for (int c = 0; c < clusters; c++){
    //     cout << ClusterArray[c]->centroid->id << endl;   
    // }

    delete[] init_centroids;

}

void KMeans::ClassicAssignment(VectorElement** Input_Array, int how_many_rows){

    //ignore thyself
    for (int i = 0; i < how_many_rows; i++){
        
        double min_dist = DBL_MAX;
        int min_id = -1;
        for (int k = 0; k < clusters; k++){

            VectorElement *vobj = Input_Array[i];
            vobj->getL2Distance(ClusterArray[k]->centroid);

            if (vobj->distanceCurrQ < min_dist){
                min_dist = vobj->distanceCurrQ;
                min_id = ClusterArray[k]->id;
            } 
        }

        ClusterArray[min_id]->cluster_elements.push_back(Input_Array[i]);
    }

    // for (int k = 0; k < clusters; k++){

    //     cout << "k is " << k << endl;
    //     list<VectorElement *>::iterator hitr1;
    //     for (hitr1 = ClusterArray[k]->cluster_elements.begin(); hitr1 != ClusterArray[k]->cluster_elements.end(); ++hitr1)
    //     {
    //         VectorElement *vobg = *hitr1;
    //         cout << "vector id" << vobg->id << endl;
    //         for (int j = 0; j < 5; j++){
    //             cout << vobg->arrayVectorElement[j] << endl;
    //         }
    //     }

    // }

}

void KMeans::ReverseAssignment(VectorElement** Input_Array, int how_many_rows){



    //find min dist among centroids
    double min_dist = DBL_MAX;
    for (int k = 0; k < clusters; k++){
        
        for (int k2 = 0; k2 < clusters; k2++){
            if (ClusterArray[k2]->id == ClusterArray[k]->id) continue;

            VectorElement* vobj = ClusterArray[k]->centroid;

            vobj->getL2Distance(ClusterArray[k2]->centroid);
            if (vobj->distanceCurrQ < min_dist){
                min_dist = vobj->distanceCurrQ;
            } 
        }
    }

    
    //ignore thyself
    
    int assigned_total = 0;
    int non_assignemnts = 0;
    double power = 1;
    while (assigned_total != how_many_rows){
        //cout << assigned_total << endl;

        for (int k = 0; k < clusters; k++){

            int size_before = ClusterArray[k]->cluster_elements.size();

            if (this->assigner == "HyperCube"){
                this->KMeans_Hyper->current_cluster = ClusterArray[k]->id; 
                this->KMeans_Hyper->range = min_dist * power; 
                this->KMeans_Hyper->range_list = ClusterArray[k]->cluster_elements;
                this->KMeans_Hyper->getFirstProbe(ClusterArray[k]->centroid, 0, "range");
                
                ClusterArray[k]->cluster_elements = this->KMeans_Hyper->range_list;
            }else{
                for (int j = 0; j < hashes; j++) //for eacrh q of the queryset
                {

                    if (j!=0){
                        this->KMeans_Hash_Array[j]->range_list = this->KMeans_Hash_Array[j-1]->range_list;        
                        this->KMeans_Hash_Array[j]->assigned_total = this->KMeans_Hash_Array[j-1]->assigned_total;      
                    }else{
                        this->KMeans_Hash_Array[j]->range_list = ClusterArray[k]->cluster_elements;
                    }

                    this->KMeans_Hash_Array[j]->current_cluster = ClusterArray[k]->id;
                    this->KMeans_Hash_Array[j]->RangeSearch(ClusterArray[k]->centroid, this->r_array, 0, min_dist * power);            
                }
                ClusterArray[k]->cluster_elements = this->KMeans_Hash_Array[hashes-1]->range_list;
            }

            int size_after = ClusterArray[k]->cluster_elements.size();
            //cout << "sizes " <<size_before << " " <<size_after << " centro" << k << endl;
            if (size_before == size_after){
            //    cout << "non ass " <<size_before << " " <<size_after << " centro" << k << endl;
                non_assignemnts++;
            }

        }
        
        for (int j = 0; j < how_many_rows; j++)
        {
            //cout << "assigned clusts " << Input_Array[j]->assigned_clusters.size() << endl;
            if (Input_Array[j]->assigned_clusters.size() > 1){

                
                double min_dist = DBL_MAX;
                int min_id = -1;
                for (list<int>::iterator i=Input_Array[j]->assigned_clusters.begin(); i!=Input_Array[j]->assigned_clusters.end(); i++){
                    int cluster_index = *i;
                    VectorElement *vobj = Input_Array[j];
                    vobj->getL2Distance(ClusterArray[cluster_index]->centroid);
                    if (vobj->distanceCurrQ < min_dist){
                        min_dist = vobj->distanceCurrQ;
                        min_id = ClusterArray[cluster_index]->id;
                    } 
            
                }   

                for (list<int>::iterator i=Input_Array[j]->assigned_clusters.begin(); i!=Input_Array[j]->assigned_clusters.end(); i++){
                    
                    int cluster_index = *i;
                    if (cluster_index == min_id) continue;
                    
                    ClusterArray[cluster_index]->cluster_elements.remove(Input_Array[j]);
                }
            }
            
        }
        
        if (this->assigner == "HyperCube"){
            assigned_total = this->KMeans_Hyper->assigned_total;
        }else{
            assigned_total = this->KMeans_Hash_Array[hashes-1]->assigned_total;
        }

        power *= 2;

        if (non_assignemnts > (clusters/2)){
            break;
        }

    }

    int count = 0;
    //cout << "assss" << assigned_total << endl;

   
    if (assigned_total != how_many_rows){
        //cout << "got to loyds" << endl;
        for (int i = 0; i < how_many_rows; i++){
            if (Input_Array[i]->assigned == false){
                count++;
                double min_dist = DBL_MAX;
                int min_id = -1;
                for (int k = 0; k < clusters; k++){

                    VectorElement *vobj = Input_Array[i];
                    vobj->getL2Distance(ClusterArray[k]->centroid);

                    //cout << "dists " << vobj->distanceCurrQ << endl;

                    if (vobj->distanceCurrQ < min_dist){
                        min_dist = vobj->distanceCurrQ;
                        min_id = ClusterArray[k]->id;
                    } 
                }
                //cout << "min id " <<min_id<<endl;
                ClusterArray[min_id]->cluster_elements.push_back(Input_Array[i]);
                //cout << "min id " <<min_id<<endl;
            }
        }
    }
    //cout << "cc " << count << endl;

    // for (int k = 0; k < clusters; k++){
    //     cout << "got to " << ClusterArray[k]->cluster_elements.size() << endl;
    // }

    // if (r_array != nullptr){
    //     delete[] r_array;
    // }


}

void KMeans::update(int columns){

    // for (int k = 0; k < clusters; k++){
    //     cout << "displaying centroid "<< k <<" before update" << endl;
    //     for (int j = 0; j < columns; j++){
    //         cout << ClusterArray[k]->centroid->arrayVectorElement[j] << endl;
    //     }

    // }

    
    for (int k = 0; k < clusters; k++){

        for (int j = 0; j < columns; j++){

            double curr_sum = 0.0;
            list<VectorElement *>::iterator hitr1;
            for (hitr1 = ClusterArray[k]->cluster_elements.begin(); hitr1 != ClusterArray[k]->cluster_elements.end(); ++hitr1)
            {
                VectorElement *vobg = *hitr1;
                curr_sum += vobg->arrayVectorElement[j];
                
            }
            ClusterArray[k]->centroid->arrayVectorElement[j] = curr_sum;
        }

    }

    for (int k = 0; k < clusters; k++){
        
        int list_size = ClusterArray[k]->cluster_elements.size();
        for (int j = 0; j < columns; j++){

            double mean = ClusterArray[k]->centroid->arrayVectorElement[j] / list_size;
            ClusterArray[k]->centroid->arrayVectorElement[j] = mean;
        }
    }



}


double KMeans::silhouette(int rows){


    if (clusters == 1){
        double silhouette_total = 0;
        return silhouette_total;
    }

    double silhouette_total = 0.0;
    for (int k = 0; k < clusters; k++){

        double silhouette_in_cluster = 0.0;

        int list_size = ClusterArray[k]->cluster_elements.size();
        int list_size_B = 0;
        for (list<VectorElement *>::iterator hitr1 = ClusterArray[k]->cluster_elements.begin(); hitr1 != ClusterArray[k]->cluster_elements.end(); ++hitr1){
            VectorElement *curr_element = *hitr1;
            
            double a_dists = 0.0;
            for (list<VectorElement *>::iterator hitr2 = ClusterArray[k]->cluster_elements.begin(); hitr2 != ClusterArray[k]->cluster_elements.end(); ++hitr2){
                VectorElement *vobg2 = *hitr2;
                if (vobg2->id == curr_element->id) continue;

                curr_element->getL2Distance(vobg2);
                a_dists += curr_element->distanceCurrQ;
            }
            a_dists = a_dists / list_size;
        

            double min_dist = DBL_MAX;
            int min_id = -1;
            for (int k2 = 0; k2 < clusters; k2++){
                if (ClusterArray[k2]->id == ClusterArray[k]->id) continue;

                curr_element->getL2Distance(ClusterArray[k2]->centroid);
                if (curr_element->distanceCurrQ < min_dist){
                    min_dist = curr_element->distanceCurrQ;
                    min_id = ClusterArray[k2]->id;
                }
            }

            double b_dists = 0.0;
            list_size_B = ClusterArray[min_id]->cluster_elements.size();
            for (list<VectorElement *>::iterator hitr3 = ClusterArray[min_id]->cluster_elements.begin(); hitr3 != ClusterArray[min_id]->cluster_elements.end(); ++hitr3){
                VectorElement *vobg3 = *hitr3;
                curr_element->getL2Distance(vobg3);
                b_dists += curr_element->distanceCurrQ;
            }
            b_dists = b_dists / list_size_B;
        

            double max_of_ab = max(a_dists,b_dists);
            double diff = b_dists - a_dists;
            double silhouette_curr = diff/max_of_ab;

            curr_element->silhouette_score = silhouette_curr;

//            cout << "silhoutte in element: " << curr_element->silhouette_score << endl;

            silhouette_in_cluster += silhouette_curr;
            silhouette_total += silhouette_curr;
        }
        if (list_size <= 1 || list_size_B <= 1){
            ClusterArray[k]->silhouette_cluster = 0;
        }else{
            ClusterArray[k]->silhouette_cluster = silhouette_in_cluster / list_size;
        }
        
        //cout << "silhoutte in cluster: " << ClusterArray[k]->silhouette_cluster << endl;
    }

    silhouette_total = silhouette_total / rows;
    //cout << "silhoutte in total: " << silhouette_total << endl;
    return silhouette_total;
}


KMeans::~KMeans(){

    for (int i = 0; i < clusters; i++){
        delete ClusterArray[i];
    }

    delete[] ClusterArray;
}