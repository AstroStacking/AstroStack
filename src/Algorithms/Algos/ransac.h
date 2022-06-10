#pragma once

#include <Eigen/Dense>
#include <Eigen/Core>

#include <numeric>
#include <random>

namespace astro
{
template<class Model>
class RANSAC
{
    using Matrix = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;
    const Matrix& m_X;
    const Matrix& m_Y;

    std::mt19937 m_generator;

    Model m_initialModel;

    int m_nbInliers{0};
    Model m_finalModel;

    const int m_nbSamples;
    const int m_nbIterations;

    void iterate()
    {
        std::vector<int> v(m_X.cols());
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), m_generator);

        v.resize(m_nbSamples);

        Model model = m_initialModel;

        Matrix Xp = m_X(Eigen::placeholders::all, v);
        Matrix Yp = m_Y(Eigen::placeholders::all, v);

        model.fit(Xp, Yp);
        auto errorp = model.predict(Xp) - Yp;
        auto reshapedError = errorp.reshaped();

        float standardDeviation = std::sqrt(std::accumulate(reshapedError.begin(), reshapedError.end(), 0.f,
                                                            [](float sum, float el) { return sum + el * el; }) /
                                            (errorp.cols() * errorp.cols()));

        auto error = model.predict(m_X) - m_Y;

        int count = (error.array().abs() <= standardDeviation).count();
        if (count > m_nbInliers)
        {
            m_nbInliers = count;
            m_finalModel = model;
        }
    }

public:
    static constexpr int DEFAULT_NB_SAMPLES = 100;
    static constexpr int DEFAULT_NB_ITERATIONS = 100;

    RANSAC(Model&& model, const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
           const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y, int nbSamples = DEFAULT_NB_SAMPLES,
           int nbIterations = DEFAULT_NB_ITERATIONS)
        : m_X(X)
        , m_Y(Y)
        , m_generator(std::random_device()())
        , m_initialModel(std::move(model))
        , m_nbSamples(nbSamples)
        , m_nbIterations(nbIterations)
    {
        if (m_X.cols() <= m_nbSamples)
        {
            throw std::range_error("X need to have more samples than the number of samples per iterations");
        }
        if (m_Y.cols() != m_X.cols())
        {
            throw std::range_error("X and Y need to have the same number of columns");
        }
    }

    void run()
    {
        m_nbInliers = 0;
        for (int i = 0; i < m_nbIterations; ++i)
        {
            iterate();
        }
    }
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>
    predict(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X) const
    {
        return m_finalModel.predict(X);
    }
};
} // namespace astro
