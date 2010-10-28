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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 05:00PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:00PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:00PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 05:00PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:44PM",
      "10/27/2010 05:00PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:54PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:54PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:55PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:47PM",
      "10/27/2010 05:02PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:42PM",
      "10/27/2010 04:57PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:58PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:00PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:01PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:45PM",
      "10/27/2010 05:01PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:46PM",
      "10/27/2010 05:02PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:58PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:43PM",
      "10/27/2010 04:59PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:41PM",
      "10/27/2010 04:57PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:39PM",
      "10/27/2010 04:55PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
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
          ]
        ]
      }
    ],
    "runTimes": [
      "10/27/2010 04:40PM",
      "10/27/2010 04:55PM"
    ],
    "commits": [
      "6d369a0e871f1a269a53cab4827bfe624595dac0",
      "6d369a0e871f1a269a53cab4827bfe624595dac0"
    ],
    "file": "sqrt_int.rb",
    "source": "require 'helpers'\n\ndef run()\n  j = 0\n  i = 0\n  while i < ITERATIONS\n    i += 1\n    j += Math.sqrt(4)\n  end\n  j\nend\n\nwith_harness do\n  run()\nend\n\n\n\n"
  }
];