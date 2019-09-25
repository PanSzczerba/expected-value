#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <functional>
#include <future>
#include <chrono>
#include <random>

#include "highest_subsequence_sum.h"

double count_expected_value(unsigned long long n, unsigned long long sample)
{
    using namespace std;
    mt19937 random_generator(chrono::system_clock::now().time_since_epoch().count() ^ hash<thread::id>()(this_thread::get_id()));
    // random_device random_generator;
    uniform_real_distribution<double> distribution(-1.0, 1.0);
    auto generate_minus_one_to_one = bind(distribution, ref(random_generator));

    vector<double> data(n);
    double average = 0;
    double sum;
    unsigned long long average_elements_count = 0;

    for(unsigned long long i = 0; i < sample; i++)
    {
        generate(data.begin(), data.end(), generate_minus_one_to_one);
        sum = find_highest_subsequence_sum(data.begin(), data.end());
        average_elements_count++;
        average += (sum - average) / average_elements_count;
    }

    return average;
}

inline double join_averages(double average1, unsigned long long average1_elements_count, double average2, unsigned long long average2_elements_count)
{
    return (average1 * average1_elements_count + average2 * average2_elements_count ) / (average1_elements_count + average2_elements_count);
}

int main(int argc, char** argv)
{
    using namespace std;

    if(argc < 3)
    {
        cerr<<"Error: too few arguments"<<endl;
        return 1;
    }
    
    unsigned future_pool_size = thread::hardware_concurrency() > 0 ? thread::hardware_concurrency() - 1 : 0; // minus one for the main thread
    if(argc >= 4)
    {
        try
        {
            future_pool_size = stoull(argv[3]);
            if(future_pool_size > 0)
                future_pool_size--; // main thread
        } catch (invalid_argument& e)
        {
            cerr<<"Error: invalid thread number: "<<argv[3]<<endl;
            return 1;
        }
    }

    unsigned long long n;
    unsigned long long sample;
    try
    {
        n = stoull(argv[1]);
        sample = stoull(argv[2]);
    } catch (invalid_argument& e)
    {
        cerr<<"Error: invalid input"<<endl;
        return 1;
    }

    future<double> future_pool[future_pool_size];
    unsigned long long batch = sample / (future_pool_size + 1);

    for(unsigned i = 0; i < future_pool_size; i++)
    {
        future_pool[i] = async(launch::async, count_expected_value, n, batch);
        sample -= batch;
    }
    
    unsigned long long average_elements_count = sample;
    double average = count_expected_value(n, sample);
    double batch_average;

    for(unsigned i = 0; i < future_pool_size; i++)
    {
        batch_average = future_pool[i].get();
        average = join_averages(average, average_elements_count, batch_average, batch);
        average_elements_count += batch;
    }

    cout<<average<<endl;
}
