clear

mvn package ### ?

ALGO_TYPE= ### java or c++
LIB_PATH= ### path to lib*.so (c++) or *.jar (java)
CLASS_NAME= ### full class name (only java)
NOTALGOAPI_PATH= ### path of NotAlgoAPI.jar (only java)

### example
# ALGO_TYPE=c++
# LIB_PATH=/libMeow.so
# CLASS_NAME=
# NOTALGOAPI_PATH=
### or
#ALGO_TYPE=java
#LIB_PATH=/MeowImpl.jar
#CLASS_NAME=impl.Impl
#NOTALGOAPI_PATH=/NotAlgoAPI.jar

INPUT_PATH=/input ### either local path or path on HDFS
OUTPUT_PATH=/output ### either local path or path on HDFS

$HADOOP_PREFIX/bin/hadoop jar target/bugfree-spice-1.0-SNAPSHOT.jar cly753/dodoopmapreduce/NotDriver $INPUT_PATH $OUTPUT_PATH $ALGO_TYPE $LIB_PATH $CLASS_NAME $NOTALGOAPI_PATH
