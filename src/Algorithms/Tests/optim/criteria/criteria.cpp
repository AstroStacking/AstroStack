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

TEST(Criteria, Monotony)
{
    optim::State state;

    optim::criteria::Monotony monotony(0.1);
    ASSERT_FALSE(monotony(state));
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);
    state.setCurrentValue(1);
    ASSERT_FALSE(monotony(state));
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);
    state.setCurrentValue(1);
    ASSERT_FALSE(monotony(state));
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);
    state.setCurrentValue(1.1);
    ASSERT_TRUE(monotony(state));
    ASSERT_EQ(state.getStatus(), optim::Status::NON_MONOTONOUS);
}

TEST(Criteria, RelativeValue)
{
    optim::State state;

    optim::criteria::RelativeValue relative(0.1);
    state.setCurrentValue(10);
    ASSERT_FALSE(relative(state));
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);
    state.setCurrentValue(11);
    ASSERT_TRUE(relative(state));
    ASSERT_EQ(state.getStatus(), optim::Status::SMALL_DELTA_F_F);
}

TEST(Criteria, AbsoluteValue)
{
    optim::State state;

    optim::criteria::AbsoluteValue absolute(0.1);
    state.setCurrentValue(1);
    ASSERT_FALSE(absolute(state));
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);
    state.setCurrentValue(.9);
    ASSERT_TRUE(absolute(state));
    ASSERT_EQ(state.getStatus(), optim::Status::SMALL_DELTA_F);
}
