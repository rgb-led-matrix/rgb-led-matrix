#TODO

## Warning
There currently is two levels here. The include folder is the public API which is only headers. The lib folder is the private API which is headers and source for the private API. The lib folder also includes the public API's source. So all implementation is in the lib folder. However the headers which are needed by the shared libraries which are built by this code base are in the include folder.