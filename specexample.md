
```
type: binary_node
package: node_package

subscriptions:
 - topic: first/topic
   type: topic/Type
 - topic: second/topic
   type: topic/Type
 - ...
publications:
 - topic: first/topic
   type: topic/Type
 - topic: second/topic
   type: topic/Type
 - ...
services:
 - name: first/service
   type: service/Type
 - name: second/service
   type: service/Type
 - ...
parameters:
 - name: firstParameter
   type: string
   default: "defaultText"
 - name: ~anotherParameter
   type: double
   default: 0.0
   range: [-1.0,1.0]
 - ...
```