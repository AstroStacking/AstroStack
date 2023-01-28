#include <Algos/Optim/Criteria/composite.h>
#include <Algos/Optim/Criteria/criteria.h>

#include <gtest/gtest.h>

TEST(Criteria, And)
{
    optim::State state;

    auto composite = optim::criteria::makeAnd(optim::criteria::Iteration(10), optim::criteria::RelativeValue(0.1));
    state.setCurrentValue(10);

    ASSERT_FALSE(composite(state));
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);

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
    ASSERT_FALSE(composite(state));
    // First criterion is check and will change status
    ASSERT_EQ(state.getStatus(), optim::Status::ITERATION_MAX_REACHED);
    state.setCurrentValue(11);
    ASSERT_TRUE(composite(state));
    ASSERT_EQ(state.getStatus(), optim::Status::SMALL_DELTA_F_F);
}

TEST(Criteria, Or)
{
    optim::State state;

    auto composite = optim::criteria::makeOr(optim::criteria::Iteration(10), optim::criteria::RelativeValue(0.1));
    state.setCurrentValue(10);

    ASSERT_FALSE(composite(state));
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);

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
    ASSERT_TRUE(composite(state));
    ASSERT_EQ(state.getStatus(), optim::Status::ITERATION_MAX_REACHED);
    state.setCurrentValue(11);
    ASSERT_TRUE(composite(state));
    // Second criterion is not even looked at, even if it would be true
    ASSERT_EQ(state.getStatus(), optim::Status::ITERATION_MAX_REACHED);
}
