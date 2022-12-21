#include <IO/hdf5.h>

#include <QtCore/QTemporaryFile>

#include <gtest/gtest.h>

class HDF5Test : public ::testing::Test
{

protected:
    QTemporaryFile temp;
    H5::H5File file;

    HDF5Test() = default;

    ~HDF5Test() override = default;

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

TEST_F(HDF5Test, CanDoBar) {}
