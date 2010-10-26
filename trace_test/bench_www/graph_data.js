var graphs = [
  {
    "name": "if_exit_in_deep_nesting1",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            203.2776
          ],
          [
            1,
            208.2258
          ],
          [
            2,
            215.1246
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            217.8122
          ],
          [
            1,
            215.8984
          ],
          [
            2,
            212.2044
          ]
        ]
      },
      {
        "name": "interp_100000",
        "data": [
          [
            0,
            254.4994
          ],
          [
            1,
            257.9474
          ],
          [
            2,
            253.078
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            272.2964
          ],
          [
            1,
            263.415
          ],
          [
            2,
            265.1838
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            269.9912
          ],
          [
            1,
            261.595
          ],
          [
            2,
            271.186
          ]
        ]
      },
      {
        "name": "trace_100000",
        "data": [
          [
            0,
            275.472
          ],
          [
            1,
            300.778
          ],
          [
            2,
            290.8368
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:35PM",
      "10/24/2010 10:40PM",
      "10/25/2010 12:31AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\n\ndef foo(i)\n  if i < ITERATIONS/2\n    true\n  else\n    false\n  end\nend\n\ndef times(i)\n  q = 0\n  while q < 1\n    q += 1\n    foo(i)\n  end\n  2\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += times(i)\n    i += 1\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "sqrt",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            206.2148
          ],
          [
            1,
            197.2148
          ],
          [
            2,
            201.0254
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            225.355
          ],
          [
            1,
            224.7332
          ],
          [
            2,
            215.6968
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            242.0074
          ],
          [
            1,
            255.0652
          ],
          [
            2,
            251.7416
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            260.9858
          ],
          [
            1,
            260.0248
          ],
          [
            2,
            258.9836
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:34PM",
      "10/24/2010 10:39PM",
      "10/25/2010 12:30AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += Math.sqrt(4.0)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "if_exit_in_two_funcs",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            209.6208
          ],
          [
            1,
            200.5242
          ],
          [
            2,
            206.758
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            212.8814
          ],
          [
            1,
            200.0344
          ],
          [
            2,
            205.2354
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            232.536
          ],
          [
            1,
            238.1426
          ],
          [
            2,
            239.5268
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            234.0866
          ],
          [
            1,
            238.1962
          ],
          [
            2,
            237.4736
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:35PM",
      "10/24/2010 10:40PM",
      "10/25/2010 12:31AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef foo(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -3\n  end\n  k\nend\n\ndef calc(i)\n  foo(i)\nend\n\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  puts \"Result: #{j}\"\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit_with_yield",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            208.354
          ],
          [
            1,
            201.8984
          ],
          [
            2,
            220.9408
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            214.118
          ],
          [
            1,
            214.667
          ],
          [
            2,
            223.789
          ]
        ]
      },
      {
        "name": "interp_100000",
        "data": [
          [
            0,
            339.6684
          ],
          [
            1,
            355.2508
          ],
          [
            2,
            348.3778
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            294.1958
          ],
          [
            1,
            314.275
          ],
          [
            2,
            296.7012
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            304.8256
          ],
          [
            1,
            298.7744
          ],
          [
            2,
            301.6998
          ]
        ]
      },
      {
        "name": "trace_100000",
        "data": [
          [
            0,
            363.765
          ],
          [
            1,
            357.1498
          ],
          [
            2,
            353.0462
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:35PM",
      "10/24/2010 10:40PM",
      "10/25/2010 12:31AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\n\ndef foo(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -2\n  end\n  k\nend\n\ndef calc(i)\n  k = 666\n  1.times do\n    k += foo(i)\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += calc(i)\n    i += 1\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "if_exit",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            193.0082
          ],
          [
            1,
            207.9694
          ],
          [
            2,
            200.4556
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            206.5502
          ],
          [
            1,
            207.7114
          ],
          [
            2,
            210.2954
          ]
        ]
      },
      {
        "name": "interp_100000",
        "data": [
          [
            0,
            242.9204
          ],
          [
            1,
            256.5734
          ],
          [
            2,
            256.2852
          ]
        ]
      },
      {
        "name": "interp_1000000",
        "data": [
          [
            0,
            653.5232
          ],
          [
            1,
            728.7382
          ],
          [
            2,
            679.7384
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            240.9466
          ],
          [
            1,
            249.6416
          ],
          [
            2,
            253.496
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            232.107
          ],
          [
            1,
            263.4682
          ],
          [
            2,
            252.22
          ]
        ]
      },
      {
        "name": "trace_100000",
        "data": [
          [
            0,
            249.0474
          ],
          [
            1,
            256.5034
          ],
          [
            2,
            253.0938
          ]
        ]
      },
      {
        "name": "trace_1000000",
        "data": [
          [
            0,
            366.936
          ],
          [
            1,
            382.1604
          ],
          [
            2,
            368.1238
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:35PM",
      "10/24/2010 10:40PM",
      "10/25/2010 12:30AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef foo()\n  23\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    if(i < (ITERATIONS/2))\n      a = 24\n      b = foo()\n      c = (b - a) * 1\n      d = c / 1\n      j += d\n    else\n      a = 24\n      b = foo()\n      c = (b - a) * 1\n      d = c / 1\n      j -= d\n    end\n  end\n  puts \"Result: #{j}\"\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "nest_funcs_with_yield",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            203.7246
          ],
          [
            1,
            223.4926
          ],
          [
            2,
            215.4396
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            203.8666
          ],
          [
            1,
            231.2378
          ],
          [
            2,
            213.6888
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            219.3476
          ],
          [
            1,
            240.4074
          ],
          [
            2,
            225.477
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            237.6338
          ],
          [
            1,
            256.6506
          ],
          [
            2,
            232.359
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:34PM",
      "10/24/2010 10:39PM",
      "10/25/2010 12:30AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\n\ndef foo()\n  k = 0\n  k.times do\n    k += 1\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += foo()\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "loop_with_puts",
    "dataSet": [
      {
        "name": "interp_100",
        "data": [
          [
            0,
            214.2262
          ],
          [
            1,
            197.3556
          ],
          [
            2,
            209.189
          ]
        ]
      },
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            214.0438
          ],
          [
            1,
            212.721
          ],
          [
            2,
            226.5652
          ]
        ]
      },
      {
        "name": "trace_100",
        "data": [
          [
            0,
            204.2406
          ],
          [
            1,
            213.2826
          ],
          [
            2,
            207.5958
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            278.06
          ],
          [
            1,
            269.6146
          ],
          [
            2,
            298.626
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:36PM",
      "10/24/2010 10:41PM",
      "10/25/2010 12:31AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    puts \".\"\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "loop_with_hash",
    "dataSet": [
      {
        "name": "interp_100",
        "data": [
          [
            0,
            197.1106
          ],
          [
            1,
            199.062
          ],
          [
            2,
            217.2194
          ]
        ]
      },
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            201.2248
          ],
          [
            1,
            199.9928
          ],
          [
            2,
            210.961
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            254.0588
          ],
          [
            1,
            241.2088
          ],
          [
            2,
            245.8676
          ]
        ]
      },
      {
        "name": "trace_100",
        "data": [
          [
            0,
            447.6806
          ],
          [
            1,
            440.5046
          ],
          [
            2,
            437.4202
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            457.7818
          ],
          [
            1,
            441.4926
          ],
          [
            2,
            446.8404
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            475.6666
          ],
          [
            1,
            467.367
          ],
          [
            2,
            484.1466
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:35PM",
      "10/24/2010 10:40PM",
      "10/25/2010 12:31AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef run()\n  hash = {}\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    hash[i] = i\n    i += 1\n    j += hash[i - 1]\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n"
  },
  {
    "name": "nest_simple",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            290.7424
          ],
          [
            1,
            290.9658
          ],
          [
            2,
            306.1
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            1081.2718
          ],
          [
            1,
            1096.1708
          ],
          [
            2,
            1096.5664
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            249.498
          ],
          [
            1,
            245.6182
          ],
          [
            2,
            248.3536
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            368.7316
          ],
          [
            1,
            387.4502
          ],
          [
            2,
            388.7962
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:34PM",
      "10/24/2010 10:39PM",
      "10/25/2010 12:30AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    k = 0\n    while k < 1000\n      j += 1\n      k += 1\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "loop",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            391.8544
          ],
          [
            1,
            299.7736
          ],
          [
            2,
            198.9764
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            198.682
          ],
          [
            1,
            212.0752
          ],
          [
            2,
            195.2912
          ]
        ]
      },
      {
        "name": "interp_100000",
        "data": [
          [
            0,
            209.7018
          ],
          [
            1,
            214.5426
          ],
          [
            2,
            207.8782
          ]
        ]
      },
      {
        "name": "interp_1000000",
        "data": [
          [
            0,
            255.8702
          ],
          [
            1,
            259.3116
          ],
          [
            2,
            264.582
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            217.12
          ],
          [
            1,
            234.6074
          ],
          [
            2,
            233.0186
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            206.9764
          ],
          [
            1,
            218.5492
          ],
          [
            2,
            219.4304
          ]
        ]
      },
      {
        "name": "trace_100000",
        "data": [
          [
            0,
            213.1306
          ],
          [
            1,
            211.412
          ],
          [
            2,
            211.5042
          ]
        ]
      },
      {
        "name": "trace_1000000",
        "data": [
          [
            0,
            230.6736
          ],
          [
            1,
            235.6604
          ],
          [
            2,
            227.8324
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:34PM",
      "10/24/2010 10:39PM",
      "10/25/2010 12:30AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n\n\n"
  },
  {
    "name": "sqrt_int",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            220.9406
          ],
          [
            1,
            210.8664
          ],
          [
            2,
            209.0446
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            248.0668
          ],
          [
            1,
            237.5396
          ],
          [
            2,
            236.0532
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            269.4196
          ],
          [
            1,
            272.5178
          ],
          [
            2,
            266.8098
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            323.5398
          ],
          [
            1,
            292.9582
          ],
          [
            2,
            296.8828
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:34PM",
      "10/24/2010 10:39PM",
      "10/25/2010 12:30AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += Math.sqrt(4)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  },
  {
    "name": "if_exit_in_func",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            198.502
          ],
          [
            1,
            213.0634
          ],
          [
            2,
            213.2058
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            205.5286
          ],
          [
            1,
            225.9122
          ],
          [
            2,
            202.9854
          ]
        ]
      },
      {
        "name": "interp_100000",
        "data": [
          [
            0,
            229.52
          ],
          [
            1,
            243.3992
          ],
          [
            2,
            231.745
          ]
        ]
      },
      {
        "name": "interp_1000000",
        "data": [
          [
            0,
            529.094
          ],
          [
            1,
            557.2112
          ],
          [
            2,
            534.4764
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            231.047
          ],
          [
            1,
            239.7128
          ],
          [
            2,
            241.0688
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            226.0512
          ],
          [
            1,
            260.8198
          ],
          [
            2,
            247.1322
          ]
        ]
      },
      {
        "name": "trace_100000",
        "data": [
          [
            0,
            229.134
          ],
          [
            1,
            249.263
          ],
          [
            2,
            241.9526
          ]
        ]
      },
      {
        "name": "trace_1000000",
        "data": [
          [
            0,
            315.4722
          ],
          [
            1,
            304.5518
          ],
          [
            2,
            308.7024
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:35PM",
      "10/24/2010 10:40PM",
      "10/25/2010 12:31AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef calc(i)\n  k = 0\n  if i < ITERATIONS/2\n    k = 2\n  else\n    k = -3\n  end\n  k\nend\n\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  j\nend\n\n\nwith_harness do\n  run()\nend\n\n"
  },
  {
    "name": "if_exit_with_yield_simple",
    "dataSet": [
      {
        "name": "interp_1000",
        "data": [
          [
            0,
            214.6158
          ],
          [
            1,
            203.2174
          ],
          [
            2,
            207.1998
          ]
        ]
      },
      {
        "name": "interp_10000",
        "data": [
          [
            0,
            229.6166
          ],
          [
            1,
            212.3148
          ],
          [
            2,
            213.6738
          ]
        ]
      },
      {
        "name": "trace_1000",
        "data": [
          [
            0,
            249.0
          ],
          [
            1,
            301.2596
          ],
          [
            2,
            258.5816
          ]
        ]
      },
      {
        "name": "trace_10000",
        "data": [
          [
            0,
            256.6714
          ],
          [
            1,
            260.5768
          ],
          [
            2,
            250.62
          ]
        ]
      }
    ],
    "runTimes": [
      "10/24/2010 10:35PM",
      "10/24/2010 10:40PM",
      "10/25/2010 12:31AM"
    ],
    "commits": [
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f",
      "482b8314839ac1f969f7004fbd8b43ebe7a8c40f"
    ],
    "source": "require 'helpers'\n\ndef calc(i)\n  1.times do\n    3\n  end\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    j += calc(i)\n    i += 1\n  end\n  j\nend\n\n\n\nwith_harness do\n  run()\nend\n\n\n\n"
  }
];