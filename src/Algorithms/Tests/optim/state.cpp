#include <Algos/optim/criteria/criteria.h>

#include <gtest/gtest.h>

TEST(State, Init)
{
    optim::State state;
    ASSERT_EQ(state.getCurrentIteration(), 0);
    ASSERT_EQ(state.getStatus(), optim::Status::NOT_STOPPED);
    ASSERT_EQ(state.getCurrentValue(), std::numeric_limits<double>::max());
    ASSERT_EQ(state.getPreviousValue(), std::numeric_limits<double>::max());
    ASSERT_EQ(state.getDirection(), Eigen::VectorXd());
    ASSERT_TRUE(std::isnan(state.getStep()));
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

TEST(State, ChangeDirection)
{
    optim::State state;
    ASSERT_EQ(state.getDirection(), Eigen::VectorXd());
    Eigen::VectorXd direction(2);
    direction << 1, 2;
    state.setDirection(direction);
    ASSERT_EQ(state.getDirection(), direction);
}

TEST(State, ChangeStep)
{
    optim::State state;
    ASSERT_TRUE(std::isnan(state.getStep()));
    state.setStep(1);
    ASSERT_EQ(state.getStep(), 1);
}
