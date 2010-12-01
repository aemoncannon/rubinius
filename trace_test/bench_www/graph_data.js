var graphs = [
  {
    "name": "if_exit - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            198.8706
          ],
          [
            1,
            198.2963
          ],
          [
            2,
            202.6913
          ],
          [
            3,
            201.3455
          ],
          [
            4,
            199.4812
          ],
          [
            5,
            197.9498
          ],
          [
            6,
            197.9473
          ],
          [
            7,
            200.7727
          ],
          [
            8,
            236.4811
          ],
          [
            9,
            233.5265
          ],
          [
            10,
            234.147
          ],
          [
            11,
            247.4524
          ],
          [
            12,
            252.1418
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            213.0894
          ],
          [
            11,
            221.2314
          ],
          [
            12,
            221.9798
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            235.3983
          ],
          [
            1,
            234.9259
          ],
          [
            2,
            241.557
          ],
          [
            3,
            236.6889
          ],
          [
            4,
            234.8039
          ],
          [
            5,
            236.6562
          ],
          [
            6,
            235.0127
          ],
          [
            7,
            240.9874
          ],
          [
            8,
            284.2321
          ],
          [
            9,
            255.132
          ],
          [
            10,
            256.0718
          ],
          [
            11,
            272.3628
          ],
          [
            12,
            273.8968
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM",
      "10/30/2010 06:36PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:35AM",
      "11/16/2010 11:43PM",
      "11/28/2010 10:40PM",
      "11/29/2010 10:24PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:41PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit.rb",
    "source": "require 'helpers'\n\ndef foo()\n  23\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    if(i < (ITERATIONS/2))\n      a = 24\n      b = foo()\n      c = (b - a) * 1\n      d = c / 1\n      j += d\n    else\n      a = 24\n      b = foo()\n      c = (b - a) * 1\n      d = c / 1\n      j -= d\n    end\n  end\n  puts \"Result: #{j}\"\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            239.8787
          ],
          [
            1,
            237.8649
          ],
          [
            2,
            247.3686
          ],
          [
            3,
            241.9039
          ],
          [
            4,
            239.0356
          ],
          [
            5,
            235.0475
          ],
          [
            6,
            233.3142
          ],
          [
            7,
            227.7218
          ],
          [
            8,
            281.6827
          ],
          [
            9,
            279.6839
          ],
          [
            10,
            278.269
          ],
          [
            11,
            292.5006
          ],
          [
            12,
            292.5808
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            241.608
          ],
          [
            11,
            253.7206
          ],
          [
            12,
            255.6528
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            246.7063
          ],
          [
            1,
            245.9016
          ],
          [
            2,
            252.4496
          ],
          [
            3,
            249.4964
          ],
          [
            4,
            247.2918
          ],
          [
            5,
            249.3459
          ],
          [
            6,
            247.6109
          ],
          [
            7,
            249.2233
          ],
          [
            8,
            331.6817
          ],
          [
            9,
            275.6617
          ],
          [
            10,
            273.401
          ],
          [
            11,
            294.0356
          ],
          [
            12,
            296.7676
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM",
      "10/30/2010 06:36PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:35AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:40PM",
      "11/29/2010 10:24PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:41PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit.rb",
    "source": "require 'helpers'\n\ndef foo()\n  23\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    if(i < (ITERATIONS/2))\n      a = 24\n      b = foo()\n      c = (b - a) * 1\n      d = c / 1\n      j += d\n    else\n      a = 24\n      b = foo()\n      c = (b - a) * 1\n      d = c / 1\n      j -= d\n    end\n  end\n  puts \"Result: #{j}\"\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit - 1000000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            647.8309
          ],
          [
            1,
            645.963
          ],
          [
            2,
            660.1836
          ],
          [
            3,
            649.4014
          ],
          [
            4,
            656.6957
          ],
          [
            5,
            612.3803
          ],
          [
            6,
            602.5828
          ],
          [
            7,
            518.9868
          ],
          [
            8,
            697.9481
          ],
          [
            9,
            723.5294
          ],
          [
            10,
            720.917
          ],
          [
            11,
            738.4024
          ],
          [
            12,
            745.1698
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            577.2488
          ],
          [
            11,
            594.4022
          ],
          [
            12,
            590.0788
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            357.1176
          ],
          [
            1,
            354.0184
          ],
          [
            2,
            357.2524
          ],
          [
            3,
            355.2532
          ],
          [
            4,
            350.9333
          ],
          [
            5,
            351.9328
          ],
          [
            6,
            352.2458
          ],
          [
            7,
            355.2981
          ],
          [
            8,
            697.8444
          ],
          [
            9,
            458.4604
          ],
          [
            10,
            461.0462
          ],
          [
            11,
            476.7812
          ],
          [
            12,
            475.3496
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:36PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:35AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:40PM",
      "11/29/2010 10:24PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:41PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit.rb",
    "source": "require 'helpers'\n\ndef foo()\n  23\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    if(i < (ITERATIONS/2))\n      a = 24\n      b = foo()\n      c = (b - a) * 1\n      d = c / 1\n      j += d\n    else\n      a = 24\n      b = foo()\n      c = (b - a) * 1\n      d = c / 1\n      j -= d\n    end\n  end\n  puts \"Result: #{j}\"\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit_in_deep_nesting1 - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            200.5233
          ],
          [
            1,
            198.1158
          ],
          [
            2,
            204.1989
          ],
          [
            3,
            202.6875
          ],
          [
            4,
            200.6093
          ],
          [
            5,
            199.1172
          ],
          [
            6,
            199.1727
          ],
          [
            7,
            201.9937
          ],
          [
            8,
            239.6843
          ],
          [
            9,
            234.7041
          ],
          [
            10,
            236.5206
          ],
          [
            11,
            255.0472
          ],
          [
            12,
            249.7596
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            208.51
          ],
          [
            11,
            221.228
          ],
          [
            12,
            223.6646
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            256.4051
          ],
          [
            1,
            258.4523
          ],
          [
            2,
            264.9175
          ],
          [
            3,
            260.0648
          ],
          [
            4,
            255.5747
          ],
          [
            5,
            255.7941
          ],
          [
            6,
            255.6148
          ],
          [
            7,
            246.3048
          ],
          [
            8,
            300.8863
          ],
          [
            9,
            264.9304
          ],
          [
            10,
            269.1806
          ],
          [
            11,
            283.9816
          ],
          [
            12,
            286.3882
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:37PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:36AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:24PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_in_deep_nesting1.rb",
    "source": "require 'helpers'\n\n\ndef foo(i)\n  if i < ITERATIONS/2\n    true\n  else\n    false\n  end\nend\n\ndef times(i)\n  q = 0\n  while q < 1\n    q += 1\n    foo(i)\n  end\n  2\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += times(i)\n    i += 1\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "if_exit_in_deep_nesting1 - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            249.9057
          ],
          [
            1,
            246.0755
          ],
          [
            2,
            257.1365
          ],
          [
            3,
            245.8902
          ],
          [
            4,
            252.7838
          ],
          [
            5,
            245.7915
          ],
          [
            6,
            243.3172
          ],
          [
            7,
            244.263
          ],
          [
            8,
            293.3514
          ],
          [
            9,
            288.4263
          ],
          [
            10,
            289.462
          ],
          [
            11,
            311.9132
          ],
          [
            12,
            313.605
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            248.4486
          ],
          [
            11,
            260.6346
          ],
          [
            12,
            266.6104
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            265.3481
          ],
          [
            1,
            266.2841
          ],
          [
            2,
            269.1621
          ],
          [
            3,
            266.8087
          ],
          [
            4,
            262.3954
          ],
          [
            5,
            264.6153
          ],
          [
            6,
            262.4624
          ],
          [
            7,
            254.147
          ],
          [
            8,
            346.0239
          ],
          [
            9,
            280.2327
          ],
          [
            10,
            282.7158
          ],
          [
            11,
            301.8098
          ],
          [
            12,
            300.0384
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:37PM",
      "10/31/2010 05:13PM",
      "11/04/2010 01:36AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:25PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_in_deep_nesting1.rb",
    "source": "require 'helpers'\n\n\ndef foo(i)\n  if i < ITERATIONS/2\n    true\n  else\n    false\n  end\nend\n\ndef times(i)\n  q = 0\n  while q < 1\n    q += 1\n    foo(i)\n  end\n  2\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += times(i)\n    i += 1\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "if_exit_in_func - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            198.0725
          ],
          [
            1,
            197.5483
          ],
          [
            2,
            200.7827
          ],
          [
            3,
            200.4649
          ],
          [
            4,
            203.0636
          ],
          [
            5,
            196.6738
          ],
          [
            6,
            194.6356
          ],
          [
            7,
            198.31
          ],
          [
            8,
            235.6981
          ],
          [
            9,
            232.0629
          ],
          [
            10,
            235.5996
          ],
          [
            11,
            249.4196
          ],
          [
            12,
            250.8478
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            207.4334
          ],
          [
            11,
            218.4278
          ],
          [
            12,
            221.1344
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            257.6785
          ],
          [
            1,
            231.3219
          ],
          [
            2,
            232.1706
          ],
          [
            3,
            228.0639
          ],
          [
            4,
            232.2705
          ],
          [
            5,
            226.4568
          ],
          [
            6,
            227.6737
          ],
          [
            7,
            228.4752
          ],
          [
            8,
            282.2899
          ],
          [
            9,
            253.0706
          ],
          [
            10,
            254.2448
          ],
          [
            11,
            270.5462
          ],
          [
            12,
            269.19
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:36PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:35AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:24PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:41PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_in_func.rb",
    "source": "require 'helpers'\n\ndef calc(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -3\n  end\n  k\nend\n\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit_in_func - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            229.3754
          ],
          [
            1,
            234.355
          ],
          [
            2,
            229.1018
          ],
          [
            3,
            229.0049
          ],
          [
            4,
            228.8615
          ],
          [
            5,
            225.1662
          ],
          [
            6,
            219.9523
          ],
          [
            7,
            221.4986
          ],
          [
            8,
            263.9851
          ],
          [
            9,
            266.7497
          ],
          [
            10,
            267.994
          ],
          [
            11,
            282.224
          ],
          [
            12,
            279.9578
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            231.1686
          ],
          [
            11,
            243.966
          ],
          [
            12,
            256.4388
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            235.4784
          ],
          [
            1,
            237.6287
          ],
          [
            2,
            234.7835
          ],
          [
            3,
            236.7864
          ],
          [
            4,
            234.3885
          ],
          [
            5,
            238.0371
          ],
          [
            6,
            232.9899
          ],
          [
            7,
            237.3972
          ],
          [
            8,
            304.8517
          ],
          [
            9,
            263.1364
          ],
          [
            10,
            262.179
          ],
          [
            11,
            282.5684
          ],
          [
            12,
            285.4666
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:36PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:35AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:24PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_in_func.rb",
    "source": "require 'helpers'\n\ndef calc(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -3\n  end\n  k\nend\n\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit_in_func - 1000000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            526.4939
          ],
          [
            1,
            526.4516
          ],
          [
            2,
            523.5725
          ],
          [
            3,
            527.6807
          ],
          [
            4,
            518.0584
          ],
          [
            5,
            473.8465
          ],
          [
            6,
            459.2464
          ],
          [
            7,
            465.5154
          ],
          [
            8,
            579.9794
          ],
          [
            9,
            580.2948
          ],
          [
            10,
            585.0762
          ],
          [
            11,
            605.4406
          ],
          [
            12,
            619.206
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            459.5018
          ],
          [
            11,
            468.8066
          ],
          [
            12,
            505.2094
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            296.8188
          ],
          [
            1,
            297.4372
          ],
          [
            2,
            301.1167
          ],
          [
            3,
            302.9015
          ],
          [
            4,
            300.5483
          ],
          [
            5,
            296.8873
          ],
          [
            6,
            295.9691
          ],
          [
            7,
            301.9814
          ],
          [
            8,
            516.4901
          ],
          [
            9,
            389.0115
          ],
          [
            10,
            388.1586
          ],
          [
            11,
            408.25
          ],
          [
            12,
            409.076
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:37PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:35AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:24PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_in_func.rb",
    "source": "require 'helpers'\n\ndef calc(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -3\n  end\n  k\nend\n\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit_in_two_funcs - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            200.3496
          ],
          [
            1,
            198.3648
          ],
          [
            2,
            204.4732
          ],
          [
            3,
            201.5611
          ],
          [
            4,
            199.5878
          ],
          [
            5,
            198.6668
          ],
          [
            6,
            195.9983
          ],
          [
            7,
            198.0646
          ],
          [
            8,
            238.4853
          ],
          [
            9,
            232.4983
          ],
          [
            10,
            234.7372
          ],
          [
            11,
            249.3266
          ],
          [
            12,
            256.4344
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            208.3848
          ],
          [
            11,
            221.7498
          ],
          [
            12,
            229.389
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            232.0039
          ],
          [
            1,
            230.3484
          ],
          [
            2,
            235.228
          ],
          [
            3,
            234.9349
          ],
          [
            4,
            230.5567
          ],
          [
            5,
            233.4918
          ],
          [
            6,
            230.1123
          ],
          [
            7,
            235.0273
          ],
          [
            8,
            291.9628
          ],
          [
            9,
            259.767
          ],
          [
            10,
            257.557
          ],
          [
            11,
            274.766
          ],
          [
            12,
            277.6244
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:37PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:35AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:24PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_in_two_funcs.rb",
    "source": "require 'helpers'\n\ndef foo(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -3\n  end\n  k\nend\n\ndef calc(i)\n  foo(i)\nend\n\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  puts \"Result: #{j}\"\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit_in_two_funcs - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            236.4096
          ],
          [
            1,
            236.8752
          ],
          [
            2,
            236.3068
          ],
          [
            3,
            241.6961
          ],
          [
            4,
            237.0476
          ],
          [
            5,
            229.759
          ],
          [
            6,
            226.5133
          ],
          [
            7,
            229.1399
          ],
          [
            8,
            274.9633
          ],
          [
            9,
            272.4489
          ],
          [
            10,
            271.5852
          ],
          [
            11,
            285.4766
          ],
          [
            12,
            292.9036
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            236.2782
          ],
          [
            11,
            248.773
          ],
          [
            12,
            268.7906
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            237.1377
          ],
          [
            1,
            237.8937
          ],
          [
            2,
            241.8143
          ],
          [
            3,
            239.9425
          ],
          [
            4,
            238.0657
          ],
          [
            5,
            238.9503
          ],
          [
            6,
            238.6599
          ],
          [
            7,
            241.299
          ],
          [
            8,
            329.2613
          ],
          [
            9,
            271.3912
          ],
          [
            10,
            267.7324
          ],
          [
            11,
            289.398
          ],
          [
            12,
            291.7156
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:37PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:36AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:24PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_in_two_funcs.rb",
    "source": "require 'helpers'\n\ndef foo(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -3\n  end\n  k\nend\n\ndef calc(i)\n  foo(i)\nend\n\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  puts \"Result: #{j}\"\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit_with_yield - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            207.2314
          ],
          [
            1,
            208.7471
          ],
          [
            2,
            210.7297
          ],
          [
            3,
            212.4272
          ],
          [
            4,
            209.0701
          ],
          [
            5,
            210.0161
          ],
          [
            6,
            205.662
          ],
          [
            7,
            211.4121
          ],
          [
            8,
            245.7933
          ],
          [
            9,
            241.9601
          ],
          [
            10,
            244.1006
          ],
          [
            11,
            255.3002
          ],
          [
            12,
            256.916
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            213.1018
          ],
          [
            11,
            232.066
          ],
          [
            12,
            229.3528
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            287.7418
          ],
          [
            1,
            290.1943
          ],
          [
            2,
            297.3082
          ],
          [
            3,
            295.1092
          ],
          [
            4,
            282.7117
          ],
          [
            5,
            284.322
          ],
          [
            6,
            306.1043
          ],
          [
            7,
            294.5715
          ],
          [
            8,
            245.6231
          ],
          [
            9,
            245.7337
          ],
          [
            10,
            242.765
          ],
          [
            11,
            258.4954
          ],
          [
            12,
            257.5244
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:08PM",
      "10/30/2010 06:37PM",
      "10/31/2010 05:13PM",
      "11/04/2010 01:36AM",
      "11/16/2010 11:45PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:25PM",
      "11/29/2010 11:33PM",
      "11/30/2010 06:02PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_with_yield.rb",
    "source": "require 'helpers'\n\n\ndef foo(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -2\n  end\n  k\nend\n\ndef calc(i)\n  k = 666\n  1.times do\n    k += foo(i)\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += calc(i)\n    i += 1\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "if_exit_with_yield - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            331.6871
          ],
          [
            1,
            332.3016
          ],
          [
            2,
            339.7915
          ],
          [
            3,
            332.8191
          ],
          [
            4,
            337.4708
          ],
          [
            5,
            335.0507
          ],
          [
            6,
            327.9297
          ],
          [
            7,
            329.3885
          ],
          [
            8,
            367.5211
          ],
          [
            9,
            368.8833
          ],
          [
            10,
            370.8526
          ],
          [
            11,
            406.259
          ],
          [
            12,
            384.596
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            325.2856
          ],
          [
            11,
            352.7272
          ],
          [
            12,
            346.8672
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            347.8794
          ],
          [
            1,
            347.0994
          ],
          [
            2,
            353.396
          ],
          [
            3,
            350.5713
          ],
          [
            4,
            339.5816
          ],
          [
            5,
            339.1406
          ],
          [
            6,
            363.2506
          ],
          [
            7,
            352.5005
          ],
          [
            8,
            399.1227
          ],
          [
            9,
            383.3508
          ],
          [
            10,
            387.484
          ],
          [
            11,
            399.3406
          ],
          [
            12,
            397.21
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:08PM",
      "10/30/2010 06:37PM",
      "10/31/2010 05:13PM",
      "11/04/2010 01:36AM",
      "11/16/2010 11:45PM",
      "11/28/2010 10:42PM",
      "11/29/2010 10:25PM",
      "11/29/2010 11:33PM",
      "11/30/2010 06:02PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_with_yield.rb",
    "source": "require 'helpers'\n\n\ndef foo(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -2\n  end\n  k\nend\n\ndef calc(i)\n  k = 666\n  1.times do\n    k += foo(i)\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += calc(i)\n    i += 1\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "if_exit_with_yield - 1000000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            1609.8611
          ],
          [
            1,
            1770.2097
          ],
          [
            2,
            1598.6501
          ],
          [
            3,
            1550.2152
          ],
          [
            4,
            1581.1168
          ],
          [
            5,
            1586.5834
          ],
          [
            6,
            1535.7644
          ],
          [
            7,
            1501.5909
          ],
          [
            8,
            1426.5678
          ],
          [
            9,
            1456.4766
          ],
          [
            10,
            1450.311
          ],
          [
            11,
            1559.7524
          ],
          [
            12,
            1501.2128
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            1154.6424
          ],
          [
            11,
            1181.1156
          ],
          [
            12,
            1174.4328
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            908.9384
          ],
          [
            1,
            901.6681
          ],
          [
            2,
            911.5298
          ],
          [
            3,
            908.8282
          ],
          [
            4,
            899.315
          ],
          [
            5,
            905.4678
          ],
          [
            6,
            932.2851
          ],
          [
            7,
            932.2064
          ],
          [
            8,
            1972.1005
          ],
          [
            9,
            1603.045
          ],
          [
            10,
            1588.4476
          ],
          [
            11,
            1611.177
          ],
          [
            12,
            1610.1864
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:09PM",
      "10/27/2010 07:08PM",
      "10/30/2010 06:38PM",
      "10/31/2010 05:13PM",
      "11/04/2010 01:36AM",
      "11/16/2010 11:45PM",
      "11/28/2010 10:42PM",
      "11/29/2010 10:25PM",
      "11/29/2010 11:33PM",
      "11/30/2010 06:02PM",
      "11/30/2010 06:43PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_with_yield.rb",
    "source": "require 'helpers'\n\n\ndef foo(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -2\n  end\n  k\nend\n\ndef calc(i)\n  k = 666\n  1.times do\n    k += foo(i)\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += calc(i)\n    i += 1\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "if_exit_with_yield_simple - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            205.0875
          ],
          [
            1,
            204.9237
          ],
          [
            2,
            205.2905
          ],
          [
            3,
            209.7772
          ],
          [
            4,
            205.6006
          ],
          [
            5,
            206.1228
          ],
          [
            6,
            200.7962
          ],
          [
            7,
            204.0544
          ],
          [
            8,
            246.4749
          ],
          [
            9,
            237.7297
          ],
          [
            10,
            237.6802
          ],
          [
            11,
            254.1046
          ],
          [
            12,
            252.2698
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            211.2282
          ],
          [
            11,
            226.8064
          ],
          [
            12,
            225.0034
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            244.5073
          ],
          [
            1,
            272.4753
          ],
          [
            2,
            244.7145
          ],
          [
            3,
            250.6657
          ],
          [
            4,
            242.4565
          ],
          [
            5,
            242.9061
          ],
          [
            6,
            242.4206
          ],
          [
            7,
            236.5686
          ],
          [
            8,
            245.0926
          ],
          [
            9,
            241.4573
          ],
          [
            10,
            237.9996
          ],
          [
            11,
            253.628
          ],
          [
            12,
            252.5886
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:37PM",
      "10/31/2010 05:13PM",
      "11/04/2010 01:36AM",
      "11/16/2010 11:44PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:25PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:02PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_with_yield_simple.rb",
    "source": "require 'helpers'\n\ndef calc(i)\n  1.times do\n    3\n  end\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += calc(i)\n    i += 1\n  end\n  j\nend\n\n\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "if_exit_with_yield_simple - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            298.1719
          ],
          [
            1,
            298.7892
          ],
          [
            2,
            297.4035
          ],
          [
            3,
            302.542
          ],
          [
            4,
            301.6275
          ],
          [
            5,
            299.6972
          ],
          [
            6,
            291.2061
          ],
          [
            7,
            294.856
          ],
          [
            8,
            329.6652
          ],
          [
            9,
            323.8512
          ],
          [
            10,
            325.3916
          ],
          [
            11,
            343.8484
          ],
          [
            12,
            343.7776
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            292.6064
          ],
          [
            11,
            308.7004
          ],
          [
            12,
            305.344
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            295.2992
          ],
          [
            1,
            291.7833
          ],
          [
            2,
            298.8717
          ],
          [
            3,
            295.4915
          ],
          [
            4,
            291.4366
          ],
          [
            5,
            291.2121
          ],
          [
            6,
            289.5383
          ],
          [
            7,
            287.4156
          ],
          [
            8,
            358.3734
          ],
          [
            9,
            342.6735
          ],
          [
            10,
            343.8632
          ],
          [
            11,
            351.8046
          ],
          [
            12,
            351.7194
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM",
      "10/30/2010 06:37PM",
      "10/31/2010 05:13PM",
      "11/04/2010 01:36AM",
      "11/16/2010 11:45PM",
      "11/28/2010 10:41PM",
      "11/29/2010 10:25PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:02PM",
      "11/30/2010 06:42PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "if_exit_with_yield_simple.rb",
    "source": "require 'helpers'\n\ndef calc(i)\n  1.times do\n    3\n  end\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += calc(i)\n    i += 1\n  end\n  j\nend\n\n\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "loop - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            199.9687
          ],
          [
            1,
            195.1443
          ],
          [
            2,
            198.7231
          ],
          [
            3,
            195.5942
          ],
          [
            4,
            197.893
          ],
          [
            5,
            194.7074
          ],
          [
            6,
            193.4271
          ],
          [
            7,
            195.244
          ],
          [
            8,
            230.9905
          ],
          [
            9,
            233.0963
          ],
          [
            10,
            230.676
          ],
          [
            11,
            244.1912
          ],
          [
            12,
            246.554
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            208.1616
          ],
          [
            11,
            490.188
          ],
          [
            12,
            239.651
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            211.1262
          ],
          [
            1,
            209.8058
          ],
          [
            2,
            214.2348
          ],
          [
            3,
            216.629
          ],
          [
            4,
            211.7549
          ],
          [
            5,
            211.9628
          ],
          [
            6,
            207.9629
          ],
          [
            7,
            210.0761
          ],
          [
            8,
            259.1674
          ],
          [
            9,
            245.4907
          ],
          [
            10,
            245.9706
          ],
          [
            11,
            261.6652
          ],
          [
            12,
            261.0324
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:54PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:02PM",
      "10/30/2010 06:32PM",
      "10/31/2010 05:07PM",
      "11/04/2010 01:31AM",
      "11/16/2010 11:39PM",
      "11/28/2010 10:33PM",
      "11/29/2010 10:16PM",
      "11/29/2010 11:26PM",
      "11/30/2010 05:55PM",
      "11/30/2010 06:36PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop.rb",
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "loop - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            203.3196
          ],
          [
            1,
            199.7591
          ],
          [
            2,
            208.2012
          ],
          [
            3,
            218.029
          ],
          [
            4,
            202.51
          ],
          [
            5,
            200.3189
          ],
          [
            6,
            200.1381
          ],
          [
            7,
            202.176
          ],
          [
            8,
            238.4754
          ],
          [
            9,
            240.3539
          ],
          [
            10,
            239.5604
          ],
          [
            11,
            253.6892
          ],
          [
            12,
            266.1796
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            210.477
          ],
          [
            11,
            224.4324
          ],
          [
            12,
            223.165
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            211.8805
          ],
          [
            1,
            209.4263
          ],
          [
            2,
            217.0144
          ],
          [
            3,
            214.5949
          ],
          [
            4,
            214.7542
          ],
          [
            5,
            210.789
          ],
          [
            6,
            208.467
          ],
          [
            7,
            212.9424
          ],
          [
            8,
            269.9356
          ],
          [
            9,
            244.256
          ],
          [
            10,
            243.1988
          ],
          [
            11,
            258.6818
          ],
          [
            12,
            258.734
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:54PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:02PM",
      "10/30/2010 06:32PM",
      "10/31/2010 05:08PM",
      "11/04/2010 01:31AM",
      "11/16/2010 11:40PM",
      "11/28/2010 10:33PM",
      "11/29/2010 10:16PM",
      "11/29/2010 11:27PM",
      "11/30/2010 05:55PM",
      "11/30/2010 06:36PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop.rb",
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "loop - 1000000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            254.0837
          ],
          [
            1,
            253.6441
          ],
          [
            2,
            260.5558
          ],
          [
            3,
            258.1192
          ],
          [
            4,
            255.8366
          ],
          [
            5,
            256.342
          ],
          [
            6,
            255.4009
          ],
          [
            7,
            255.6723
          ],
          [
            8,
            317.6329
          ],
          [
            9,
            321.6744
          ],
          [
            10,
            323.3774
          ],
          [
            11,
            351.2002
          ],
          [
            12,
            348.076
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            253.786
          ],
          [
            11,
            269.2864
          ],
          [
            12,
            271.324
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            223.0979
          ],
          [
            1,
            220.2584
          ],
          [
            2,
            223.5396
          ],
          [
            3,
            223.1337
          ],
          [
            4,
            221.3433
          ],
          [
            5,
            225.784
          ],
          [
            6,
            220.2744
          ],
          [
            7,
            222.3411
          ],
          [
            8,
            308.359
          ],
          [
            9,
            289.3565
          ],
          [
            10,
            287.08
          ],
          [
            11,
            300.7874
          ],
          [
            12,
            300.6654
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:02PM",
      "10/30/2010 06:32PM",
      "10/31/2010 05:08PM",
      "11/04/2010 01:31AM",
      "11/16/2010 11:40PM",
      "11/28/2010 10:33PM",
      "11/29/2010 10:16PM",
      "11/29/2010 11:27PM",
      "11/30/2010 05:55PM",
      "11/30/2010 06:36PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop.rb",
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "loop_with_changing_target - 1000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            192.5738
          ],
          [
            1,
            199.493
          ],
          [
            2,
            234.2261
          ],
          [
            3,
            229.5238
          ],
          [
            4,
            234.9586
          ],
          [
            5,
            246.7942
          ],
          [
            6,
            244.4798
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          [
            4,
            205.4218
          ],
          [
            5,
            218.4888
          ],
          [
            6,
            233.1012
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            221.2553
          ],
          [
            1,
            236.7048
          ],
          [
            2,
            265.398
          ],
          [
            3,
            249.9284
          ],
          [
            4,
            248.0714
          ],
          [
            5,
            264.4264
          ],
          [
            6,
            269.7498
          ]
        ]
      }
    ],
    "runTimes": [
      "11/04/2010 01:40AM",
      "11/16/2010 11:49PM",
      "11/28/2010 10:46PM",
      "11/29/2010 10:30PM",
      "11/29/2010 11:36PM",
      "11/30/2010 06:05PM",
      "11/30/2010 06:46PM"
    ],
    "commits": [
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop_with_changing_target.rb",
    "source": "require 'helpers'\n\n\nclass Cat\n  def legs\n    4\n  end\nend\n\nclass Chimp\n  def legs\n    2\n  end\nend\n\nclass Mollusk\n  def legs\n    1\n  end\nend\n\n\ndef foo(target)\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += target.legs\n  end\n  j\nend\n\n\ndef run()\n  j = 0\n  j += foo(Cat.new)\n  j += foo(Chimp.new)\n  j += foo(Mollusk.new)\n  puts \"result #{j}\"\n  j\nend\n\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_changing_target - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            194.5737
          ],
          [
            1,
            202.0109
          ],
          [
            2,
            239.0049
          ],
          [
            3,
            234.6753
          ],
          [
            4,
            237.2358
          ],
          [
            5,
            253.04
          ],
          [
            6,
            250.6034
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          [
            4,
            210.9238
          ],
          [
            5,
            220.1686
          ],
          [
            6,
            227.4926
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            221.8088
          ],
          [
            1,
            235.5668
          ],
          [
            2,
            265.7317
          ],
          [
            3,
            252.2143
          ],
          [
            4,
            250.952
          ],
          [
            5,
            262.7276
          ],
          [
            6,
            269.7794
          ]
        ]
      }
    ],
    "runTimes": [
      "11/04/2010 01:40AM",
      "11/16/2010 11:49PM",
      "11/28/2010 10:47PM",
      "11/29/2010 10:30PM",
      "11/29/2010 11:36PM",
      "11/30/2010 06:05PM",
      "11/30/2010 06:46PM"
    ],
    "commits": [
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop_with_changing_target.rb",
    "source": "require 'helpers'\n\n\nclass Cat\n  def legs\n    4\n  end\nend\n\nclass Chimp\n  def legs\n    2\n  end\nend\n\nclass Mollusk\n  def legs\n    1\n  end\nend\n\n\ndef foo(target)\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += target.legs\n  end\n  j\nend\n\n\ndef run()\n  j = 0\n  j += foo(Cat.new)\n  j += foo(Chimp.new)\n  j += foo(Mollusk.new)\n  puts \"result #{j}\"\n  j\nend\n\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_div_by_zero - 1000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            212.1374
          ],
          [
            1,
            208.4144
          ],
          [
            2,
            212.3609
          ],
          [
            3,
            245.4509
          ],
          [
            4,
            241.5089
          ],
          [
            5,
            246.64
          ],
          [
            6,
            254.267
          ],
          [
            7,
            254.77
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          [
            5,
            215.327
          ],
          [
            6,
            228.7954
          ],
          [
            7,
            226.1162
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            240.8324
          ],
          [
            1,
            215.5359
          ],
          [
            2,
            210.7522
          ],
          [
            3,
            246.3429
          ],
          [
            4,
            242.391
          ],
          [
            5,
            240.8682
          ],
          [
            6,
            251.4992
          ],
          [
            7,
            254.1936
          ]
        ]
      }
    ],
    "runTimes": [
      "10/31/2010 05:16PM",
      "11/04/2010 01:39AM",
      "11/16/2010 11:48PM",
      "11/28/2010 10:46PM",
      "11/29/2010 10:29PM",
      "11/29/2010 11:36PM",
      "11/30/2010 06:05PM",
      "11/30/2010 06:45PM"
    ],
    "commits": [
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop_with_div_by_zero.rb",
    "source": "require 'helpers'\n\ndef run()\n  hash = {}\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    begin\n      j += 1 / 0\n    rescue Exception => e\n      j += 1\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_div_by_zero - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            345.7323
          ],
          [
            1,
            342.362
          ],
          [
            2,
            343.7349
          ],
          [
            3,
            376.721
          ],
          [
            4,
            372.2427
          ],
          [
            5,
            371.689
          ],
          [
            6,
            380.431
          ],
          [
            7,
            386.869
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          [
            5,
            332.4956
          ],
          [
            6,
            344.312
          ],
          [
            7,
            347.2404
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            388.6775
          ],
          [
            1,
            405.9345
          ],
          [
            2,
            344.7523
          ],
          [
            3,
            375.663
          ],
          [
            4,
            373.7715
          ],
          [
            5,
            370.1952
          ],
          [
            6,
            379.5826
          ],
          [
            7,
            386.4126
          ]
        ]
      }
    ],
    "runTimes": [
      "10/31/2010 05:16PM",
      "11/04/2010 01:39AM",
      "11/16/2010 11:48PM",
      "11/28/2010 10:46PM",
      "11/29/2010 10:29PM",
      "11/29/2010 11:36PM",
      "11/30/2010 06:05PM",
      "11/30/2010 06:45PM"
    ],
    "commits": [
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop_with_div_by_zero.rb",
    "source": "require 'helpers'\n\ndef run()\n  hash = {}\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    begin\n      j += 1 / 0\n    rescue Exception => e\n      j += 1\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_hash - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            239.1856
          ],
          [
            1,
            239.9275
          ],
          [
            2,
            238.5429
          ],
          [
            3,
            242.6833
          ],
          [
            4,
            241.8862
          ],
          [
            5,
            237.9649
          ],
          [
            6,
            234.6929
          ],
          [
            7,
            238.5936
          ],
          [
            8,
            278.5856
          ],
          [
            9,
            274.8699
          ],
          [
            10,
            278.8018
          ],
          [
            11,
            288.943
          ],
          [
            12,
            285.9612
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            241.9516
          ],
          [
            11,
            255.0268
          ],
          [
            12,
            253.469
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            456.0378
          ],
          [
            1,
            459.8132
          ],
          [
            2,
            470.4453
          ],
          [
            3,
            460.9662
          ],
          [
            4,
            431.9822
          ],
          [
            5,
            442.7148
          ],
          [
            6,
            447.9833
          ],
          [
            7,
            6177.1834
          ],
          [
            8,
            369.9121
          ],
          [
            9,
            320.1023
          ],
          [
            10,
            319.3912
          ],
          [
            11,
            335.6316
          ],
          [
            12,
            340.8376
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:10PM",
      "10/27/2010 07:10PM",
      "10/30/2010 06:39PM",
      "10/31/2010 05:15PM",
      "11/04/2010 01:38AM",
      "11/16/2010 11:47PM",
      "11/28/2010 10:45PM",
      "11/29/2010 10:28PM",
      "11/29/2010 11:35PM",
      "11/30/2010 06:04PM",
      "11/30/2010 06:45PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop_with_hash.rb",
    "source": "require 'helpers'\n\ndef run()\n  hash = {}\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    hash[i] = i\n    i += 1\n    j += hash[i - 1]\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_hash - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            696.942
          ],
          [
            1,
            705.2479
          ],
          [
            2,
            707.5219
          ],
          [
            3,
            708.5011
          ],
          [
            4,
            705.7214
          ],
          [
            5,
            709.8826
          ],
          [
            6,
            699.8617
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            814.6359
          ],
          [
            1,
            814.0059
          ],
          [
            2,
            834.7312
          ],
          [
            3,
            821.6457
          ],
          [
            4,
            787.6911
          ],
          [
            5,
            782.2775
          ],
          [
            6,
            798.3106
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:11PM",
      "10/27/2010 07:10PM",
      "10/30/2010 06:40PM",
      "10/31/2010 05:15PM",
      "11/04/2010 01:39AM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb"
    ],
    "file": "loop_with_hash.rb",
    "source": "require 'helpers'\n\ndef run()\n  hash = {}\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    hash[i] = i\n    i += 1\n    j += hash[i - 1]\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_puts - 100 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            198.1716
          ],
          [
            1,
            197.1539
          ],
          [
            2,
            200.8326
          ],
          [
            3,
            202.1015
          ],
          [
            4,
            198.8615
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            204.1237
          ],
          [
            1,
            214.8575
          ],
          [
            2,
            200.0195
          ],
          [
            3,
            203.2019
          ],
          [
            4,
            199.5735
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:11PM",
      "10/27/2010 07:10PM",
      "10/30/2010 06:40PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0"
    ],
    "file": "loop_with_puts.rb",
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    puts \".\"\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_puts - 1000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            213.4173
          ],
          [
            1,
            209.6094
          ],
          [
            2,
            216.0565
          ],
          [
            3,
            213.07
          ],
          [
            4,
            210.8232
          ],
          [
            5,
            209.7834
          ],
          [
            6,
            209.5915
          ],
          [
            7,
            210.6969
          ],
          [
            8,
            251.6154
          ],
          [
            9,
            242.986
          ],
          [
            10,
            248.0156
          ],
          [
            11,
            256.0946
          ],
          [
            12,
            262.0956
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            222.2392
          ],
          [
            11,
            233.6526
          ],
          [
            12,
            231.1534
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            278.2521
          ],
          [
            1,
            268.0114
          ],
          [
            2,
            235.7718
          ],
          [
            3,
            271.639
          ],
          [
            4,
            266.2089
          ],
          [
            5,
            264.6546
          ],
          [
            6,
            269.8225
          ],
          [
            7,
            215.0237
          ],
          [
            8,
            247.4436
          ],
          [
            9,
            244.3085
          ],
          [
            10,
            244.1912
          ],
          [
            11,
            255.9632
          ],
          [
            12,
            258.228
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:11PM",
      "10/27/2010 07:10PM",
      "10/30/2010 06:40PM",
      "10/31/2010 05:15PM",
      "11/04/2010 01:39AM",
      "11/16/2010 11:48PM",
      "11/28/2010 10:45PM",
      "11/29/2010 10:29PM",
      "11/29/2010 11:35PM",
      "11/30/2010 06:04PM",
      "11/30/2010 06:45PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop_with_puts.rb",
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    puts \".\"\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_puts - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            343.1037
          ],
          [
            1,
            339.0781
          ],
          [
            2,
            344.2974
          ],
          [
            3,
            383.6156
          ],
          [
            4,
            376.4361
          ],
          [
            5,
            377.4722
          ],
          [
            6,
            400.2586
          ],
          [
            7,
            386.5554
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          [
            5,
            355.608
          ],
          [
            6,
            368.9662
          ],
          [
            7,
            364.6168
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            903.545
          ],
          [
            1,
            966.4571
          ],
          [
            2,
            358.9688
          ],
          [
            3,
            388.5761
          ],
          [
            4,
            385.9527
          ],
          [
            5,
            389.3438
          ],
          [
            6,
            397.737
          ],
          [
            7,
            400.9332
          ]
        ]
      }
    ],
    "runTimes": [
      "10/31/2010 05:15PM",
      "11/04/2010 01:39AM",
      "11/16/2010 11:48PM",
      "11/28/2010 10:45PM",
      "11/29/2010 10:29PM",
      "11/29/2010 11:35PM",
      "11/30/2010 06:04PM",
      "11/30/2010 06:45PM"
    ],
    "commits": [
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop_with_puts.rb",
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    puts \".\"\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_string_ops - 1000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            231.2344
          ],
          [
            1,
            258.7567
          ],
          [
            2,
            255.9374
          ],
          [
            3,
            262.7042
          ],
          [
            4,
            273.0264
          ],
          [
            5,
            271.395
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          [
            3,
            229.6152
          ],
          [
            4,
            237.337
          ],
          [
            5,
            238.7888
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            235.4556
          ],
          [
            1,
            268.1738
          ],
          [
            2,
            262.0986
          ],
          [
            3,
            259.802
          ],
          [
            4,
            269.7806
          ],
          [
            5,
            268.9272
          ]
        ]
      }
    ],
    "runTimes": [
      "11/16/2010 11:48PM",
      "11/28/2010 10:45PM",
      "11/29/2010 10:29PM",
      "11/29/2010 11:35PM",
      "11/30/2010 06:04PM",
      "11/30/2010 06:45PM"
    ],
    "commits": [
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop_with_string_ops.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    a = \"apples and oranges\".gsub(\"e\", \"q\")\n    b = a.include?(\"es\")\n    i += 1\n    if b\n      j += 1\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_string_ops - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            510.3763
          ],
          [
            1,
            536.5254
          ],
          [
            2,
            527.0098
          ],
          [
            3,
            532.6182
          ],
          [
            4,
            548.7696
          ],
          [
            5,
            540.9786
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          [
            3,
            431.6778
          ],
          [
            4,
            445.512
          ],
          [
            5,
            456.0932
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            539.3929
          ],
          [
            1,
            588.7276
          ],
          [
            2,
            541.3849
          ],
          [
            3,
            549.8216
          ],
          [
            4,
            547.2312
          ],
          [
            5,
            555.2294
          ]
        ]
      }
    ],
    "runTimes": [
      "11/16/2010 11:48PM",
      "11/28/2010 10:46PM",
      "11/29/2010 10:29PM",
      "11/29/2010 11:35PM",
      "11/30/2010 06:05PM",
      "11/30/2010 06:45PM"
    ],
    "commits": [
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "loop_with_string_ops.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    a = \"apples and oranges\".gsub(\"e\", \"q\")\n    b = a.include?(\"es\")\n    i += 1\n    if b\n      j += 1\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "nest_funcs - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            754.0414
          ],
          [
            1,
            757.3282
          ],
          [
            2,
            733.7547
          ],
          [
            3,
            763.0551
          ],
          [
            4,
            735.5358
          ],
          [
            5,
            726.5815
          ],
          [
            6,
            721.4954
          ],
          [
            7,
            737.8675
          ],
          [
            8,
            972.5794
          ],
          [
            9,
            1115.4268
          ],
          [
            10,
            1118.9082
          ],
          [
            11,
            1172.7658
          ],
          [
            12,
            1188.9152
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            603.6528
          ],
          [
            11,
            614.067
          ],
          [
            12,
            616.9594
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            322.5511
          ],
          [
            1,
            336.9402
          ],
          [
            2,
            331.0814
          ],
          [
            3,
            333.4744
          ],
          [
            4,
            331.9034
          ],
          [
            5,
            328.1625
          ],
          [
            6,
            333.8559
          ],
          [
            7,
            322.8245
          ],
          [
            8,
            796.0778
          ],
          [
            9,
            725.8682
          ],
          [
            10,
            723.0132
          ],
          [
            11,
            738.8038
          ],
          [
            12,
            751.857
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:42PM",
      "10/27/2010 04:57PM",
      "10/27/2010 06:06PM",
      "10/27/2010 07:05PM",
      "10/30/2010 06:35PM",
      "10/31/2010 05:10PM",
      "11/04/2010 01:33AM",
      "11/16/2010 11:42PM",
      "11/28/2010 10:38PM",
      "11/29/2010 10:21PM",
      "11/29/2010 11:30PM",
      "11/30/2010 05:59PM",
      "11/30/2010 06:39PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs.rb",
    "source": "require 'helpers'\n\n\ndef foo()\n  k = 0\n  while k < 1000\n    k += 1\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += foo()\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "nest_funcs - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            5787.8576
          ],
          [
            1,
            5777.2259
          ],
          [
            2,
            5518.811
          ],
          [
            3,
            5852.6066
          ],
          [
            4,
            5552.8654
          ],
          [
            5,
            5430.4332
          ],
          [
            6,
            5473.7655
          ],
          [
            7,
            5578.6677
          ],
          [
            8,
            7601.7698
          ],
          [
            9,
            9137.3988
          ],
          [
            10,
            9128.2628
          ],
          [
            11,
            9732.6358
          ],
          [
            12,
            9666.4544
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            4201.9144
          ],
          [
            11,
            4203.1518
          ],
          [
            12,
            4196.605
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            1270.0751
          ],
          [
            1,
            1261.5147
          ],
          [
            2,
            1262.7769
          ],
          [
            3,
            1300.8517
          ],
          [
            4,
            1296.9211
          ],
          [
            5,
            1269.1315
          ],
          [
            6,
            1324.287
          ],
          [
            7,
            1219.6611
          ],
          [
            8,
            5707.6872
          ],
          [
            9,
            5141.7478
          ],
          [
            10,
            5176.0124
          ],
          [
            11,
            5112.995
          ],
          [
            12,
            5174.075
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:58PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM",
      "10/30/2010 06:36PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:34AM",
      "11/16/2010 11:43PM",
      "11/28/2010 10:40PM",
      "11/29/2010 10:22PM",
      "11/29/2010 11:31PM",
      "11/30/2010 06:00PM",
      "11/30/2010 06:40PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs.rb",
    "source": "require 'helpers'\n\n\ndef foo()\n  k = 0\n  while k < 1000\n    k += 1\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += foo()\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "nest_funcs_and_exit - 1000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            226.771
          ],
          [
            1,
            226.94
          ],
          [
            2,
            230.2182
          ],
          [
            3,
            226.4293
          ],
          [
            4,
            224.7423
          ],
          [
            5,
            224.9564
          ],
          [
            6,
            227.8697
          ],
          [
            7,
            226.3766
          ],
          [
            8,
            262.19
          ],
          [
            9,
            261.504
          ],
          [
            10,
            265.5534
          ],
          [
            11,
            277.6156
          ],
          [
            12,
            277.2322
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            233.1332
          ],
          [
            11,
            249.7292
          ],
          [
            12,
            248.2046
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            259.6973
          ],
          [
            1,
            256.829
          ],
          [
            2,
            259.8901
          ],
          [
            3,
            255.6067
          ],
          [
            4,
            255.7832
          ],
          [
            5,
            257.0293
          ],
          [
            6,
            256.7163
          ],
          [
            7,
            315.6705
          ],
          [
            8,
            323.5338
          ],
          [
            9,
            268.7432
          ],
          [
            10,
            267.89
          ],
          [
            11,
            286.0348
          ],
          [
            12,
            283.9824
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:09PM",
      "10/27/2010 07:08PM",
      "10/30/2010 06:38PM",
      "10/31/2010 05:13PM",
      "11/04/2010 01:37AM",
      "11/16/2010 11:45PM",
      "11/28/2010 10:42PM",
      "11/29/2010 10:25PM",
      "11/29/2010 11:33PM",
      "11/30/2010 06:02PM",
      "11/30/2010 06:43PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs_and_exit.rb",
    "source": "require 'helpers'\n\ndef foo(i)\n  if i < ITERATIONS * 0.5\n    2\n  else\n    1\n  end\nend\n\ndef calc(i)\n  k = 0\n  while k < 100\n    k += foo(i)\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "nest_funcs_and_exit - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            521.7445
          ],
          [
            1,
            513.3546
          ],
          [
            2,
            524.1184
          ],
          [
            3,
            515.6043
          ],
          [
            4,
            504.806
          ],
          [
            5,
            507.0352
          ],
          [
            6,
            474.9343
          ],
          [
            7,
            490.301
          ],
          [
            8,
            493.0137
          ],
          [
            9,
            496.5122
          ],
          [
            10,
            496.2676
          ],
          [
            11,
            520.1802
          ],
          [
            12,
            519.5966
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            425.961
          ],
          [
            11,
            433.076
          ],
          [
            12,
            436.2028
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            435.7732
          ],
          [
            1,
            430.0809
          ],
          [
            2,
            438.059
          ],
          [
            3,
            429.8885
          ],
          [
            4,
            428.3782
          ],
          [
            5,
            429.5391
          ],
          [
            6,
            427.41
          ],
          [
            7,
            496.1595
          ],
          [
            8,
            631.0443
          ],
          [
            9,
            426.2847
          ],
          [
            10,
            433.1328
          ],
          [
            11,
            438.4032
          ],
          [
            12,
            443.8074
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:01PM",
      "10/27/2010 06:09PM",
      "10/27/2010 07:08PM",
      "10/30/2010 06:38PM",
      "10/31/2010 05:14PM",
      "11/04/2010 01:37AM",
      "11/16/2010 11:45PM",
      "11/28/2010 10:42PM",
      "11/29/2010 10:26PM",
      "11/29/2010 11:33PM",
      "11/30/2010 06:02PM",
      "11/30/2010 06:43PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs_and_exit.rb",
    "source": "require 'helpers'\n\ndef foo(i)\n  if i < ITERATIONS * 0.5\n    2\n  else\n    1\n  end\nend\n\ndef calc(i)\n  k = 0\n  while k < 100\n    k += foo(i)\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "nest_funcs_and_throw_exception - 1000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            211.3141
          ],
          [
            1,
            218.3073
          ],
          [
            2,
            245.0914
          ],
          [
            3,
            247.234
          ],
          [
            4,
            250.4738
          ],
          [
            5,
            260.4664
          ],
          [
            6,
            257.386
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          [
            4,
            217.4024
          ],
          [
            5,
            228.7464
          ],
          [
            6,
            241.8186
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            247.6025
          ],
          [
            1,
            252.1873
          ],
          [
            2,
            275.0074
          ],
          [
            3,
            253.4813
          ],
          [
            4,
            251.755
          ],
          [
            5,
            271.145
          ],
          [
            6,
            282.7174
          ]
        ]
      }
    ],
    "runTimes": [
      "11/04/2010 01:40AM",
      "11/16/2010 11:49PM",
      "11/28/2010 10:46PM",
      "11/29/2010 10:30PM",
      "11/29/2010 11:36PM",
      "11/30/2010 06:05PM",
      "11/30/2010 06:46PM"
    ],
    "commits": [
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs_and_throw_exception.rb",
    "source": "require 'helpers'\n\ndef foo(i)\n  if i < ITERATIONS * 0.5\n    2\n  else\n    3 / 0\n  end\nend\n\ndef calc(i)\n  k = 0\n  while k < 100\n    k += foo(i)\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    begin\n      i += 1\n      j += calc(i)\n    rescue Exception => e\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "nest_funcs_and_throw_exception - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            369.2157
          ],
          [
            1,
            377.6329
          ],
          [
            2,
            401.0035
          ],
          [
            3,
            398.528
          ],
          [
            4,
            406.3986
          ],
          [
            5,
            416.111
          ],
          [
            6,
            419.8986
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          [
            4,
            351.3372
          ],
          [
            5,
            363.0324
          ],
          [
            6,
            384.7112
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            384.2458
          ],
          [
            1,
            383.8312
          ],
          [
            2,
            484.4374
          ],
          [
            3,
            392.1433
          ],
          [
            4,
            392.9048
          ],
          [
            5,
            398.9168
          ],
          [
            6,
            415.1046
          ]
        ]
      }
    ],
    "runTimes": [
      "11/04/2010 01:40AM",
      "11/16/2010 11:49PM",
      "11/28/2010 10:46PM",
      "11/29/2010 10:30PM",
      "11/29/2010 11:36PM",
      "11/30/2010 06:05PM",
      "11/30/2010 06:46PM"
    ],
    "commits": [
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs_and_throw_exception.rb",
    "source": "require 'helpers'\n\ndef foo(i)\n  if i < ITERATIONS * 0.5\n    2\n  else\n    3 / 0\n  end\nend\n\ndef calc(i)\n  k = 0\n  while k < 100\n    k += foo(i)\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    begin\n      i += 1\n      j += calc(i)\n    rescue Exception => e\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "nest_funcs_with_optional_args - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            763.4914
          ],
          [
            1,
            760.6519
          ],
          [
            2,
            780.9815
          ],
          [
            3,
            756.7648
          ],
          [
            4,
            768.6383
          ],
          [
            5,
            737.2161
          ],
          [
            6,
            870.2159
          ],
          [
            7,
            732.4994
          ],
          [
            8,
            998.5493
          ],
          [
            9,
            1143.2153
          ],
          [
            10,
            1149.835
          ],
          [
            11,
            1136.146
          ],
          [
            12,
            1142.1074
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            731.6326
          ],
          [
            11,
            739.6556
          ],
          [
            12,
            741.905
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            333.4881
          ],
          [
            1,
            333.3551
          ],
          [
            2,
            337.8946
          ],
          [
            3,
            328.0921
          ],
          [
            4,
            339.694
          ],
          [
            5,
            341.0822
          ],
          [
            6,
            335.9912
          ],
          [
            7,
            342.3722
          ],
          [
            8,
            783.5742
          ],
          [
            9,
            716.2067
          ],
          [
            10,
            719.3916
          ],
          [
            11,
            725.6644
          ],
          [
            12,
            735.7348
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:01PM",
      "10/27/2010 06:09PM",
      "10/27/2010 07:09PM",
      "10/30/2010 06:38PM",
      "10/31/2010 05:14PM",
      "11/04/2010 01:37AM",
      "11/16/2010 11:46PM",
      "11/28/2010 10:43PM",
      "11/29/2010 10:26PM",
      "11/29/2010 11:33PM",
      "11/30/2010 06:03PM",
      "11/30/2010 06:43PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs_with_optional_args.rb",
    "source": "require 'helpers'\n\n\ndef foo(m = 1)\n  k = 0\n  while k < 1000\n    k += m\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += foo()\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "nest_funcs_with_optional_args - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            5821.1831
          ],
          [
            1,
            5831.0136
          ],
          [
            2,
            5969.2934
          ],
          [
            3,
            5898.9757
          ],
          [
            4,
            5972.2727
          ],
          [
            5,
            5601.9905
          ],
          [
            6,
            6957.785
          ],
          [
            7,
            5576.3523
          ],
          [
            8,
            7943.4769
          ],
          [
            9,
            9478.1837
          ],
          [
            10,
            9284.0806
          ],
          [
            11,
            9164.4338
          ],
          [
            12,
            9225.2658
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            5399.7194
          ],
          [
            11,
            5428.3782
          ],
          [
            12,
            5392.534
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            1279.1416
          ],
          [
            1,
            1323.4016
          ],
          [
            2,
            1316.1328
          ],
          [
            3,
            1304.5054
          ],
          [
            4,
            1425.3985
          ],
          [
            5,
            1368.4531
          ],
          [
            6,
            1349.7153
          ],
          [
            7,
            1356.1271
          ],
          [
            8,
            5513.6732
          ],
          [
            9,
            5048.1018
          ],
          [
            10,
            5054.3748
          ],
          [
            11,
            5046.9704
          ],
          [
            12,
            5063.0192
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:46PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:10PM",
      "10/27/2010 07:10PM",
      "10/30/2010 06:39PM",
      "10/31/2010 05:15PM",
      "11/04/2010 01:38AM",
      "11/16/2010 11:47PM",
      "11/28/2010 10:44PM",
      "11/29/2010 10:28PM",
      "11/29/2010 11:35PM",
      "11/30/2010 06:03PM",
      "11/30/2010 06:45PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs_with_optional_args.rb",
    "source": "require 'helpers'\n\n\ndef foo(m = 1)\n  k = 0\n  while k < 1000\n    k += m\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += foo()\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "nest_funcs_with_yield - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            206.576
          ],
          [
            1,
            205.2012
          ],
          [
            2,
            205.652
          ],
          [
            3,
            206.4073
          ],
          [
            4,
            202.2692
          ],
          [
            5,
            203.8985
          ],
          [
            6,
            203.1404
          ],
          [
            7,
            202.7154
          ],
          [
            8,
            238.8964
          ],
          [
            9,
            236.7429
          ],
          [
            10,
            237.993
          ],
          [
            11,
            249.4074
          ],
          [
            12,
            255.9228
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            210.3918
          ],
          [
            11,
            221.0266
          ],
          [
            12,
            223.1928
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            229.6527
          ],
          [
            1,
            227.5298
          ],
          [
            2,
            228.2324
          ],
          [
            3,
            227.0644
          ],
          [
            4,
            228.2301
          ],
          [
            5,
            227.7803
          ],
          [
            6,
            224.2307
          ],
          [
            7,
            234.9614
          ],
          [
            8,
            287.6384
          ],
          [
            9,
            258.249
          ],
          [
            10,
            257.2014
          ],
          [
            11,
            272.9026
          ],
          [
            12,
            272.7124
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:58PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM",
      "10/30/2010 06:36PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:35AM",
      "11/16/2010 11:43PM",
      "11/28/2010 10:40PM",
      "11/29/2010 10:23PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:41PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs_with_yield.rb",
    "source": "require 'helpers'\n\n\ndef foo()\n  k = 0\n  k.times do\n    k += 1\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += foo()\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "nest_funcs_with_yield - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            284.3822
          ],
          [
            1,
            286.3261
          ],
          [
            2,
            280.937
          ],
          [
            3,
            285.6825
          ],
          [
            4,
            280.3279
          ],
          [
            5,
            281.0101
          ],
          [
            6,
            278.7372
          ],
          [
            7,
            275.27
          ],
          [
            8,
            312.0522
          ],
          [
            9,
            312.8496
          ],
          [
            10,
            311.0364
          ],
          [
            11,
            326.482
          ],
          [
            12,
            331.2984
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            280.0594
          ],
          [
            11,
            292.1994
          ],
          [
            12,
            293.4266
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            274.5338
          ],
          [
            1,
            275.6824
          ],
          [
            2,
            281.9075
          ],
          [
            3,
            278.2536
          ],
          [
            4,
            274.6283
          ],
          [
            5,
            272.9039
          ],
          [
            6,
            271.9018
          ],
          [
            7,
            277.3152
          ],
          [
            8,
            378.4717
          ],
          [
            9,
            302.9789
          ],
          [
            10,
            301.5614
          ],
          [
            11,
            321.2348
          ],
          [
            12,
            325.6536
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM",
      "10/30/2010 06:36PM",
      "10/31/2010 05:12PM",
      "11/04/2010 01:35AM",
      "11/16/2010 11:43PM",
      "11/28/2010 10:40PM",
      "11/29/2010 10:23PM",
      "11/29/2010 11:32PM",
      "11/30/2010 06:01PM",
      "11/30/2010 06:41PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_funcs_with_yield.rb",
    "source": "require 'helpers'\n\n\ndef foo()\n  k = 0\n  k.times do\n    k += 1\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += foo()\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "nest_simple - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            1044.6469
          ],
          [
            1,
            1055.8904
          ],
          [
            2,
            1055.2276
          ],
          [
            3,
            1065.9322
          ],
          [
            4,
            1031.23
          ],
          [
            5,
            1177.5618
          ],
          [
            6,
            997.6223
          ],
          [
            7,
            993.1651
          ],
          [
            8,
            1563.6575
          ],
          [
            9,
            1651.495
          ],
          [
            10,
            1649.824
          ],
          [
            11,
            1816.8594
          ],
          [
            12,
            1816.8902
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            980.1852
          ],
          [
            11,
            1004.5984
          ],
          [
            12,
            981.5084
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            372.8457
          ],
          [
            1,
            371.3542
          ],
          [
            2,
            375.1251
          ],
          [
            3,
            374.0998
          ],
          [
            4,
            355.0994
          ],
          [
            5,
            359.3301
          ],
          [
            6,
            344.5762
          ],
          [
            7,
            361.0974
          ],
          [
            8,
            1045.7376
          ],
          [
            9,
            943.1645
          ],
          [
            10,
            949.5066
          ],
          [
            11,
            933.2952
          ],
          [
            12,
            946.7762
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:04PM",
      "10/27/2010 07:03PM",
      "10/30/2010 06:33PM",
      "10/31/2010 05:08PM",
      "11/04/2010 01:32AM",
      "11/16/2010 11:40PM",
      "11/28/2010 10:34PM",
      "11/29/2010 10:17PM",
      "11/29/2010 11:27PM",
      "11/30/2010 05:56PM",
      "11/30/2010 06:37PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_simple.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    k = 0\n    while k < 1000\n      j += 1\n      k += 1\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "nest_simple - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            8736.3351
          ],
          [
            1,
            8747.3619
          ],
          [
            2,
            8657.6613
          ],
          [
            3,
            8776.4332
          ],
          [
            4,
            8543.6516
          ],
          [
            5,
            9978.9473
          ],
          [
            6,
            8253.8966
          ],
          [
            7,
            8089.1268
          ],
          [
            8,
            13455.4011
          ],
          [
            9,
            14441.4003
          ],
          [
            10,
            14419.0052
          ],
          [
            11,
            16008.6246
          ],
          [
            12,
            15992.9488
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            7839.7722
          ],
          [
            11,
            7832.6434
          ],
          [
            12,
            7823.0368
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            1685.9116
          ],
          [
            1,
            1654.8128
          ],
          [
            2,
            1687.2215
          ],
          [
            3,
            1688.761
          ],
          [
            4,
            1553.693
          ],
          [
            5,
            1517.58
          ],
          [
            6,
            1445.7666
          ],
          [
            7,
            1569.4138
          ],
          [
            8,
            8009.8186
          ],
          [
            9,
            7233.7181
          ],
          [
            10,
            7214.9946
          ],
          [
            11,
            7260.8842
          ],
          [
            12,
            7122.815
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:41PM",
      "10/27/2010 04:57PM",
      "10/27/2010 06:06PM",
      "10/27/2010 07:05PM",
      "10/30/2010 06:34PM",
      "10/31/2010 05:10PM",
      "11/04/2010 01:33AM",
      "11/16/2010 11:42PM",
      "11/28/2010 10:37PM",
      "11/29/2010 10:19PM",
      "11/29/2010 11:29PM",
      "11/30/2010 05:58PM",
      "11/30/2010 06:38PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nest_simple.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    k = 0\n    while k < 1000\n      j += 1\n      k += 1\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "nested_funcs_with_div_by_zero - 1000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            255.4065
          ],
          [
            1,
            268.2574
          ],
          [
            2,
            303.0252
          ],
          [
            3,
            312.1033
          ],
          [
            4,
            315.4762
          ],
          [
            5,
            327.307
          ],
          [
            6,
            334.0684
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          [
            4,
            260.512
          ],
          [
            5,
            288.332
          ],
          [
            6,
            272.6502
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            240.8349
          ],
          [
            1,
            235.5574
          ],
          [
            2,
            316.9978
          ],
          [
            3,
            291.8968
          ],
          [
            4,
            287.2218
          ],
          [
            5,
            306.2998
          ],
          [
            6,
            302.0694
          ]
        ]
      }
    ],
    "runTimes": [
      "11/04/2010 01:39AM",
      "11/16/2010 11:48PM",
      "11/28/2010 10:46PM",
      "11/29/2010 10:29PM",
      "11/29/2010 11:36PM",
      "11/30/2010 06:05PM",
      "11/30/2010 06:45PM"
    ],
    "commits": [
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nested_funcs_with_div_by_zero.rb",
    "source": "require 'helpers'\n\n\ndef foo()\n  k = 0\n  while k < 1000\n    k += 1\n    if k > 500\n      k += k / 0\n    end\n  end\n  k\nend\n\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    begin\n      j += foo()\n    rescue Exception => e\n      j += 1\n    end\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "nested_funcs_with_div_by_zero - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            792.6596
          ],
          [
            1,
            865.2054
          ],
          [
            2,
            949.0714
          ],
          [
            3,
            1070.2556
          ],
          [
            4,
            1075.695
          ],
          [
            5,
            1130.4592
          ],
          [
            6,
            1127.4942
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          [
            4,
            768.8222
          ],
          [
            5,
            781.3172
          ],
          [
            6,
            784.296
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            533.0271
          ],
          [
            1,
            450.3276
          ],
          [
            2,
            829.2822
          ],
          [
            3,
            747.0466
          ],
          [
            4,
            750.9104
          ],
          [
            5,
            773.4646
          ],
          [
            6,
            762.818
          ]
        ]
      }
    ],
    "runTimes": [
      "11/04/2010 01:39AM",
      "11/16/2010 11:49PM",
      "11/28/2010 10:46PM",
      "11/29/2010 10:30PM",
      "11/29/2010 11:36PM",
      "11/30/2010 06:05PM",
      "11/30/2010 06:46PM"
    ],
    "commits": [
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nested_funcs_with_div_by_zero.rb",
    "source": "require 'helpers'\n\n\ndef foo()\n  k = 0\n  while k < 1000\n    k += 1\n    if k > 500\n      k += k / 0\n    end\n  end\n  k\nend\n\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    begin\n      j += foo()\n    rescue Exception => e\n      j += 1\n    end\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "nsieve - 4 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            1695.4128
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          [
            0,
            1677.2574
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            958.4994
          ]
        ]
      }
    ],
    "runTimes": [
      "11/30/2010 06:34PM"
    ],
    "commits": [
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nsieve.rb",
    "source": "require 'helpers'\n\ndef sieve(m)\n  flags = Flags.dup[0,m]\n  count = 0\n  pmax = m - 1\n  p = 2\n  while p <= pmax\n    unless flags[p].zero?\n      count += 1\n      mult = p\n      while mult <= pmax\n        flags[mult] = 0\n        mult += p\n      end\n    end\n    p += 1\n  end\n  count\nend\n\nm = 9\nFlags = \"\\x1\" * ( 2 ** m * 10_000)\nn = ITERATIONS\n\nwith_harness do\n\n  n.downto(n-2) do |exponent|\n    break if exponent < 0\n    m = (1 << exponent) * 10_000\n    # m = (2 ** exponent) * 10_000\n    count = sieve(m)\n    printf \"Primes up to %8d %8d\\n\", m, count\n  end\n\nend\n\n\n"
  },
  {
    "name": "nsieve - 6 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            6240.2578
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          [
            0,
            6248.9842
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            3003.9604
          ]
        ]
      }
    ],
    "runTimes": [
      "11/30/2010 06:35PM"
    ],
    "commits": [
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "nsieve.rb",
    "source": "require 'helpers'\n\ndef sieve(m)\n  flags = Flags.dup[0,m]\n  count = 0\n  pmax = m - 1\n  p = 2\n  while p <= pmax\n    unless flags[p].zero?\n      count += 1\n      mult = p\n      while mult <= pmax\n        flags[mult] = 0\n        mult += p\n      end\n    end\n    p += 1\n  end\n  count\nend\n\nm = 9\nFlags = \"\\x1\" * ( 2 ** m * 10_000)\nn = ITERATIONS\n\nwith_harness do\n\n  n.downto(n-2) do |exponent|\n    break if exponent < 0\n    m = (1 << exponent) * 10_000\n    # m = (2 ** exponent) * 10_000\n    count = sieve(m)\n    printf \"Primes up to %8d %8d\\n\", m, count\n  end\n\nend\n\n\n"
  },
  {
    "name": "sqrt - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            216.481
          ],
          [
            1,
            216.717
          ],
          [
            2,
            219.2767
          ],
          [
            3,
            218.5077
          ],
          [
            4,
            214.3721
          ],
          [
            5,
            215.8664
          ],
          [
            6,
            216.2957
          ],
          [
            7,
            219.2058
          ],
          [
            8,
            249.9222
          ],
          [
            9,
            248.197
          ],
          [
            10,
            246.549
          ],
          [
            11,
            268.9752
          ],
          [
            12,
            260.377
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            223.3656
          ],
          [
            11,
            235.3822
          ],
          [
            12,
            233.4998
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            255.3701
          ],
          [
            1,
            252.451
          ],
          [
            2,
            259.6929
          ],
          [
            3,
            257.4263
          ],
          [
            4,
            254.8502
          ],
          [
            5,
            254.0358
          ],
          [
            6,
            254.7064
          ],
          [
            7,
            259.8971
          ],
          [
            8,
            253.5951
          ],
          [
            9,
            248.1208
          ],
          [
            10,
            247.3708
          ],
          [
            11,
            259.8182
          ],
          [
            12,
            260.7634
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:02PM",
      "10/30/2010 06:32PM",
      "10/31/2010 05:08PM",
      "11/04/2010 01:31AM",
      "11/16/2010 11:40PM",
      "11/28/2010 10:33PM",
      "11/29/2010 10:16PM",
      "11/29/2010 11:27PM",
      "11/30/2010 05:56PM",
      "11/30/2010 06:36PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "sqrt.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += Math.sqrt(4.0)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "sqrt - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            446.0546
          ],
          [
            1,
            410.2189
          ],
          [
            2,
            406.8602
          ],
          [
            3,
            406.2643
          ],
          [
            4,
            413.2735
          ],
          [
            5,
            401.214
          ],
          [
            6,
            411.5082
          ],
          [
            7,
            410.3989
          ],
          [
            8,
            417.2757
          ],
          [
            9,
            410.4788
          ],
          [
            10,
            410.4288
          ],
          [
            11,
            427.553
          ],
          [
            12,
            429.656
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            395.7322
          ],
          [
            11,
            428.1506
          ],
          [
            12,
            405.1374
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            382.3641
          ],
          [
            1,
            383.8275
          ],
          [
            2,
            396.876
          ],
          [
            3,
            381.4528
          ],
          [
            4,
            382.0826
          ],
          [
            5,
            379.8451
          ],
          [
            6,
            383.4197
          ],
          [
            7,
            394.6183
          ],
          [
            8,
            461.7019
          ],
          [
            9,
            441.8356
          ],
          [
            10,
            438.7082
          ],
          [
            11,
            435.951
          ],
          [
            12,
            445.2528
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:03PM",
      "10/30/2010 06:32PM",
      "10/31/2010 05:08PM",
      "11/04/2010 01:31AM",
      "11/16/2010 11:40PM",
      "11/28/2010 10:33PM",
      "11/29/2010 10:16PM",
      "11/29/2010 11:27PM",
      "11/30/2010 05:56PM",
      "11/30/2010 06:36PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "sqrt.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += Math.sqrt(4.0)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "sqrt_int - 10000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            229.4087
          ],
          [
            1,
            228.5167
          ],
          [
            2,
            233.1566
          ],
          [
            3,
            231.8746
          ],
          [
            4,
            233.2968
          ],
          [
            5,
            236.2744
          ],
          [
            6,
            227.6756
          ],
          [
            7,
            228.6346
          ],
          [
            8,
            262.5168
          ],
          [
            9,
            258.1515
          ],
          [
            10,
            262.7792
          ],
          [
            11,
            284.4034
          ],
          [
            12,
            272.722
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            242.3944
          ],
          [
            11,
            257.576
          ],
          [
            12,
            254.939
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            284.9588
          ],
          [
            1,
            284.1524
          ],
          [
            2,
            292.5402
          ],
          [
            3,
            284.0464
          ],
          [
            4,
            282.2743
          ],
          [
            5,
            280.4455
          ],
          [
            6,
            284.8838
          ],
          [
            7,
            295.3302
          ],
          [
            8,
            268.3572
          ],
          [
            9,
            264.2957
          ],
          [
            10,
            262.5898
          ],
          [
            11,
            277.529
          ],
          [
            12,
            272.2472
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:03PM",
      "10/30/2010 06:32PM",
      "10/31/2010 05:08PM",
      "11/04/2010 01:31AM",
      "11/16/2010 11:40PM",
      "11/28/2010 10:33PM",
      "11/29/2010 10:16PM",
      "11/29/2010 11:27PM",
      "11/30/2010 05:56PM",
      "11/30/2010 06:36PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "sqrt_int.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += Math.sqrt(4)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "sqrt_int - 100000 iterations",
    "dataSet": [
      {
        "name": "interp",
        "data": [
          [
            0,
            547.4296
          ],
          [
            1,
            551.562
          ],
          [
            2,
            550.6572
          ],
          [
            3,
            546.423
          ],
          [
            4,
            557.3737
          ],
          [
            5,
            546.3976
          ],
          [
            6,
            547.589
          ],
          [
            7,
            532.8783
          ],
          [
            8,
            535.2098
          ],
          [
            9,
            533.2266
          ],
          [
            10,
            529.1718
          ],
          [
            11,
            546.2618
          ],
          [
            12,
            537.9836
          ]
        ]
      },
      {
        "name": "master_interp",
        "data": [
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          null,
          [
            10,
            499.6518
          ],
          [
            11,
            515.4756
          ],
          [
            12,
            520.9634
          ]
        ]
      },
      {
        "name": "trace",
        "data": [
          [
            0,
            476.5532
          ],
          [
            1,
            477.1346
          ],
          [
            2,
            489.7411
          ],
          [
            3,
            472.0461
          ],
          [
            4,
            473.6965
          ],
          [
            5,
            465.9088
          ],
          [
            6,
            472.7214
          ],
          [
            7,
            489.1957
          ],
          [
            8,
            631.9016
          ],
          [
            9,
            566.2379
          ],
          [
            10,
            573.1912
          ],
          [
            11,
            575.298
          ],
          [
            12,
            564.6892
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:03PM",
      "10/30/2010 06:32PM",
      "10/31/2010 05:08PM",
      "11/04/2010 01:32AM",
      "11/16/2010 11:40PM",
      "11/28/2010 10:33PM",
      "11/29/2010 10:17PM",
      "11/29/2010 11:27PM",
      "11/30/2010 05:56PM",
      "11/30/2010 06:36PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f",
      "668a0f876ea1273a23e31856f1b0d869c53e55f0",
      "df11426152f31a94b89337495b772f1ddfe58a09",
      "63c789303dd10237fca56a15a1d3c6fe2fb3e3fb",
      "e93ac31bb7691d59fbd892e9a32d926818b39184",
      "e0e4a820f573b4ad51398e0635df59e93735f612",
      "1093750ea1ee6be3b06484760b83a9b4e43a8847",
      "f7ebe7dd0447ca765b9a28ad667a5905c2a9cd59",
      "c03ba16e9dfd9af26787d34db6238ccb60fdd6ee",
      "9dfd9db97c51d06b57e874c930c8f2662c1405e6"
    ],
    "file": "sqrt_int.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += Math.sqrt(4)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  }
];