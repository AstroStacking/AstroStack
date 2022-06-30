#include <Algos/optim/criteria/criteria.h>

#include <gtest/gtest.h>

TEST(Criteria, Iteration)
{
    optim::State state;
    state.increaseIteration();
    state.increaseIteration();
    state.increaseIteration();
    state.increaseIteration();
    state.increaseIteration();
    state.increaseIteration();
    state.increaseIteration();
    state.increaseIteration();
    state.increaseIteration();
    state.increaseIteration();

    optim::criteria::Iteration iteration(20);
    ASSERT_FALSE(iteration(state));
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);
    iteration = optim::criteria::Iteration(10);
    ASSERT_TRUE(iteration(state));
    ASSERT_EQ(state.getStatus(), optim::Status::ITERATION_MAX_REACHED);
}
