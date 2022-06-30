#include <Algos/optim/criteria/criteria.h>

#include <gtest/gtest.h>

TEST(State, Init)
{
    optim::State state;
    ASSERT_EQ(state.getCurrentIteration(), 0);
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);
    ASSERT_EQ(state.getCurrentValue(), std::numeric_limits<double>::max());
    ASSERT_EQ(state.getPreviousValue(), std::numeric_limits<double>::max());
}

TEST(State, IncreaseIteration)
{
    optim::State state;
    ASSERT_EQ(state.getCurrentIteration(), 0);
    state.increaseIteration();
    ASSERT_EQ(state.getCurrentIteration(), 1);
}

TEST(State, ChangeStatus)
{
    optim::State state;
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);
    state.setStatus(optim::Status::ITERATION_MAX_REACHED);
    ASSERT_EQ(state.getStatus(), optim::Status::ITERATION_MAX_REACHED);
}

TEST(State, ChangeValue)
{
    optim::State state;
    ASSERT_EQ(state.getCurrentValue(), std::numeric_limits<double>::max());
    ASSERT_EQ(state.getPreviousValue(), std::numeric_limits<double>::max());
    state.setCurrentValue(1);
    ASSERT_EQ(state.getCurrentValue(), 1);
    ASSERT_EQ(state.getPreviousValue(), std::numeric_limits<double>::max());
    state.setCurrentValue(2);
    ASSERT_EQ(state.getCurrentValue(), 2);
    ASSERT_EQ(state.getPreviousValue(), 1);
}
