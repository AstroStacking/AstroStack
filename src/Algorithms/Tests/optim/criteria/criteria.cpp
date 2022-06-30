#include <Algos/optim/criteria/criteria.h>

#include <gtest/gtest.h>

TEST(Criteria, Iteration)
{
    struct Test
    {
        size_t iteration() const { return 10; }
        optim::criteria::StopReason m_reason{optim::criteria::StopReason::NOT_STOPPED};

        void setStopReason(optim::criteria::StopReason reason) { m_reason = reason; }
    };

    Test test;
    optim::criteria::Iteration iteration(20);
    ASSERT_FALSE(iteration(test));
    ASSERT_EQ(test.m_reason, optim::criteria::StopReason::NOT_STOPPED);
    iteration = optim::criteria::Iteration(10);
    ASSERT_TRUE(iteration(test));
    ASSERT_EQ(test.m_reason, optim::criteria::StopReason::ITERATION_MAX_REACHED);
}
