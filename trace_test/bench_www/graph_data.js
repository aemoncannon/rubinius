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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:08PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:08PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:09PM",
      "10/27/2010 07:08PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:08PM",
      "10/27/2010 07:07PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:54PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:02PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:54PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:02PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:02PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
    ],
    "file": "loop.rb",
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n\n\n"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:10PM",
      "10/27/2010 07:10PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:11PM",
      "10/27/2010 07:10PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:11PM",
      "10/27/2010 07:10PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:11PM",
      "10/27/2010 07:10PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
    ],
    "file": "loop_with_puts.rb",
    "source": "require 'helpers'\n\ndef run()\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    puts \".\"\n  end\n  i\nend\n\nwith_harness do\n  run()\nend\n"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:42PM",
      "10/27/2010 04:57PM",
      "10/27/2010 06:06PM",
      "10/27/2010 07:05PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:58PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:00PM",
      "10/27/2010 06:09PM",
      "10/27/2010 07:08PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:01PM",
      "10/27/2010 06:09PM",
      "10/27/2010 07:08PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
    ],
    "file": "nest_funcs_and_exit.rb",
    "source": "require 'helpers'\n\ndef foo(i)\n  if i < ITERATIONS * 0.5\n    2\n  else\n    1\n  end\nend\n\ndef calc(i)\n  k = 0\n  while k < 100\n    k += foo(i)\n  end\n  k\nend\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += calc(i)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:01PM",
      "10/27/2010 06:09PM",
      "10/27/2010 07:09PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:46PM",
      "10/27/2010 05:02PM",
      "10/27/2010 06:10PM",
      "10/27/2010 07:10PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:58PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:59PM",
      "10/27/2010 06:07PM",
      "10/27/2010 07:06PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:04PM",
      "10/27/2010 07:03PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:41PM",
      "10/27/2010 04:57PM",
      "10/27/2010 06:06PM",
      "10/27/2010 07:05PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
    ],
    "file": "nest_simple.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    k = 0\n    while k < 1000\n      j += 1\n      k += 1\n    end\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:02PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:03PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:03PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM",
      "10/27/2010 06:03PM",
      "10/27/2010 07:03PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "e89a5975806773eb0931457475ca3769e3b963f5",
      "861c0455641c7fbcaf7056eb801f33536e49001f"
    ],
    "file": "sqrt_int.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += Math.sqrt(4)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  }
];