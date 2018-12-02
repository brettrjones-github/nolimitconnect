import unittest
from vmaf.core.asset import Asset
from vmaf.core.executor import Executor

__copyright__ = "Copyright 2016-2017, Netflix, Inc."
__license__ = "Apache, Version 2.0"

class ExecutorTest(unittest.TestCase):

    def test_get_workfile_yuv_type(self):

        asset = Asset(dataset="test", content_id=0, asset_id=0, ref_path="", dis_path="",
                      asset_dict={}, workdir_root="my_workdir_root")
        self.assertEquals(Executor._get_workfile_yuv_type(asset), 'yuv420p')

        asset = Asset(dataset="test", content_id=0, asset_id=0, ref_path="", dis_path="",
                      asset_dict={'ref_yuv_type': 'notyuv', 'dis_yuv_type': 'notyuv'}, workdir_root="my_workdir_root")
        self.assertEquals(Executor._get_workfile_yuv_type(asset), 'yuv420p')

        asset = Asset(dataset="test", content_id=0, asset_id=0, ref_path="", dis_path="",
                      asset_dict={'ref_yuv_type': 'yuv444p', 'dis_yuv_type': 'notyuv'}, workdir_root="my_workdir_root")
        self.assertEquals(Executor._get_workfile_yuv_type(asset), 'yuv444p')

        with self.assertRaises(AssertionError):
            asset = Asset(dataset="test", content_id=0, asset_id=0, ref_path="", dis_path="",
                          asset_dict={'ref_yuv_type': 'yuv444p', 'dis_yuv_type': 'yuv420p'}, workdir_root="my_workdir_root")
            self.assertEquals(Executor._get_workfile_yuv_type(asset), 'yuv444p')

        asset = Asset(dataset="test", content_id=0, asset_id=0, ref_path="", dis_path="",
                      asset_dict={'ref_yuv_type': 'notyuv', 'dis_yuv_type': 'yuv422p'}, workdir_root="my_workdir_root")
        self.assertEquals(Executor._get_workfile_yuv_type(asset), 'yuv422p')

        asset = Asset(dataset="test", content_id=0, asset_id=0, ref_path="", dis_path="",
                      asset_dict={'ref_yuv_type': 'yuv444p', 'dis_yuv_type': 'yuv444p'}, workdir_root="my_workdir_root")
        self.assertEquals(Executor._get_workfile_yuv_type(asset), 'yuv444p')
