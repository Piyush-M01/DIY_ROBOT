# CMake generated Testfile for 
# Source directory: /home/piyush/diy_bot/src/diff_drive
# Build directory: /home/piyush/diy_bot/build/diff_drive
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_diff_drive_nosetests_tests "/home/piyush/diy_bot/build/catkin_generated/env_cached.sh" "/usr/bin/python3" "/opt/ros/noetic/share/catkin/cmake/test/run_tests.py" "/home/piyush/diy_bot/build/test_results/diff_drive/nosetests-tests.xml" "--return-code" "\"/usr/bin/cmake\" -E make_directory /home/piyush/diy_bot/build/test_results/diff_drive" "/usr/bin/nosetests3 -P --process-timeout=60 --where=/home/piyush/diy_bot/src/diff_drive/tests --with-xunit --xunit-file=/home/piyush/diy_bot/build/test_results/diff_drive/nosetests-tests.xml")
set_tests_properties(_ctest_diff_drive_nosetests_tests PROPERTIES  _BACKTRACE_TRIPLES "/opt/ros/noetic/share/catkin/cmake/test/tests.cmake;160;add_test;/opt/ros/noetic/share/catkin/cmake/test/nosetests.cmake;83;catkin_run_tests_target;/home/piyush/diy_bot/src/diff_drive/CMakeLists.txt;21;catkin_add_nosetests;/home/piyush/diy_bot/src/diff_drive/CMakeLists.txt;0;")
