#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>

#include <numeric>
#include <random>

namespace astro
{
template<class Model, int XSize = Eigen::Dynamic, int YSize = Eigen::Dynamic>
class RANSAC
{
    using MatrixX = Eigen::Matrix<double, XSize, Eigen::Dynamic>;
    using MatrixY = Eigen::Matrix<double, XSize, Eigen::Dynamic>;
    const MatrixX& m_X;
    const MatrixY& m_Y;

    std::mt19937_64 m_generator;

    Model m_initialModel;

    int m_nbInliers{0};
    Model m_finalModel;
    std::vector<int> m_seeds;

    const int m_nbSamples;
    const int m_nbIterations;

    void iterate()
    {
        std::vector<int> v(m_X.cols());
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), m_generator);

        v.resize(m_nbSamples);

        Model model = m_initialModel;

        MatrixX Xp = m_X(Eigen::placeholders::all, v);
        MatrixY Yp = m_Y(Eigen::placeholders::all, v);

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
            m_seeds = std::move(v);
        }
    }

public:
    static constexpr int DEFAULT_NB_SAMPLES = 100;
    static constexpr int DEFAULT_NB_ITERATIONS = 100;

    RANSAC(Model&& model, const MatrixX& X, const MatrixY& Y, int nbSamples = DEFAULT_NB_SAMPLES,
           int nbIterations = DEFAULT_NB_ITERATIONS, int_fast64_t seed = std::random_device()())
        : m_X(X)
        , m_Y(Y)
        , m_generator(seed)
        , m_initialModel(std::move(model))
        , m_finalModel(m_initialModel)
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

    MatrixY predict(const MatrixX& X) const { return m_finalModel.predict(X); }
    const std::vector<int>& getBestSeeds() const { return m_seeds; }
};
} // namespace astro
