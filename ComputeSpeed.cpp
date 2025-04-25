#include "ComputeSpeed.hpp"

float average(vector<float> v) {
    if (v.empty()) {
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return reduce(v.begin(), v.end()) / count;
}

void compute_speed() {
    unordered_map<int, vector<float> > cr_times;
    unordered_map<int, vector<float> > cnn_times;
    unordered_map<int, float> means_cr;
    unordered_map<int, float> means_cnn;

    vector<int> sizes = {10, 20, 40, 60, 80, 100};
    cout << "Computing..." << endl;
    const clock_t begin_compute = clock();
    for (int size : sizes) {
        cout << "Taille " << size << endl;
        cr_times[size] = vector<float>();
        cnn_times[size] = vector<float>();
        for (int seed = 0; seed < 50; seed++) {
            Graphe g = initKCCTP(size, size-2, 5, 30, seed);

            const clock_t begin_christo = clock();
            vector<int> christo = christofides(g);
            float time_christo = clock() - begin_christo;

            const clock_t begin_cr = clock();
            vector<int> p1 = cyclicRouting(g, christo);
            float time_cr = clock() - begin_cr + time_christo;
            cr_times.at(size).push_back(time_cr);

            const clock_t begin_cnn = clock();
            vector<int> p2 = cnn(g, christo);
            float time_cnn = clock() - begin_cnn + time_christo;
            cnn_times.at(size).push_back(time_cnn);
        }
        means_cr[size] = average(cr_times.at(size));
        cout << "moyenne en secondes de 50 executions de cr sur des graphes de taille " << size << " : " << means_cr[size]/CLOCKS_PER_SEC << endl;

        means_cnn[size] = average(cnn_times.at(size));
        cout << "moyenne en secondes de 50 executions de cnn sur des graphes de taille " << size << " : " << means_cnn[size]/CLOCKS_PER_SEC << endl << endl;
    }
    cout << "computation in " << (clock() - begin_compute)/CLOCKS_PER_SEC << " seconds" << endl;
}