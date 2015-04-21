### distributed image processing on hadoop

#### A very rough guide...

##### dependency:
- Apache Maven (version xx)
- Apache Hadoop 2.6.0 (?) [Click me for installation and setup](https://hadoop.apache.org/docs/r2.6.0/hadoop-project-dist/hadoop-common/SingleCluster.html)
- ??? (can't think of now ...)

##### how to run:
1. implement API in your algorithm
-- c++: refer to script/cpp/SomeImpl.{cpp, hpp}
--- run generate_dynamic_library.sh to generate lib*.so
-- java: refer to script/java/NotAlgoImpl.jar
--- implement interface cly753.process.NotAlgoInterface (NotAlgoAPI.jar)

2. fill missing setting in ./runDodoop.sh

3. (chmod...) run ./runDodoop.sh
