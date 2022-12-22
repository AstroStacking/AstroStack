#include <IO/hdf5.h>

#include <QtCore/QTemporaryFile>

#include <gtest/gtest.h>

class HDF5TestSuite : public ::testing::Test
{

protected:
    QTemporaryFile temp;
    H5::H5File file;

    HDF5TestSuite() = default;

    ~HDF5TestSuite() override = default;

    void SetUp() override
    {
        if (temp.open())
        {
            temp.close();
            file = H5::H5File(temp.fileName().toStdString(), H5F_ACC_TRUNC);
        }
    }

    void TearDown() override {}
};
