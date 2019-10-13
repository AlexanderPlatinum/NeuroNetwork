#pragma once

#include <QDebug>
#include <string.h>
#include <cmath>

class Network {
private:
    int width;

    float *synapticWeight[4];

public:
    Network(int _width) {
        this->width = _width;

        allocateMemory();
    }

    ~Network() {
        clearMemory();
    }

    void Teaching(float *data, int idSolution, float resultNeed) {

        for (int i = 0; i < 10000; i++) {
            float result = 0.0f;

            for (int t = 0; t < width; t++) {
                result += data[t] * synapticWeight[idSolution][t];
            }

            result = sigmoid(result);

            float error = resultNeed - result;

            //qDebug() << error;

            for (int t = 0; t < width; t++) {
                synapticWeight[idSolution][t] += data[t] * (error * (result * (1.0f - result)));
            }
        }
    }

    float Recognize(float *data, int idSolution) {
        float result = 0.0f;
        for (int i = 0; i < width; i++) {
            result += data[i] * synapticWeight[idSolution][i];
        }

        return sigmoid(result);
    }

private:

    void allocateMemory() {
        for (int i = 0; i < 4; i++) {
            synapticWeight[i] = new float[width];

            memset(synapticWeight[i], 0, sizeof(float) * width);
        }
    }

    void clearMemory () {
        for (int i = 0; i < 4; i++) {
            delete [] synapticWeight[i];
        }
    }

    float sigmoid(float x) {
        return 1.0f / (1.0f + std::exp(-x));
    }
};
