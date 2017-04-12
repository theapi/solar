/**
 * Experimenting with Google Protocol Buffer in its nanopb form:
 * https://github.com/nanopb/nanopb
 * 
 * https://github.com/nanopb/nanopb/tree/master/examples/simple
 * https://blog.noser.com/arduino-iot-with-mqtt-and-nanopb/
 * https://github.com/amorellgarcia/arduino-nanopb/tree/master/example
 */

#include <pb_encode.h>
#include <pb_decode.h>
#include "GardenSensors.pb.h"

#define NUM_SENSORS 2

// sensors
   Sensor vals[NUM_SENSORS];
   

bool sensorValues_callback(pb_ostream_t *stream, 
            const pb_field_t *field, 
            void * const *arg) {
  for (int i=0; i<NUM_SENSORS; ++i) {
    
        if (!pb_encode_tag_for_field(stream, field)) return false;
        if (!pb_encode_submessage(stream, 
                       Sensor_fields, 
                       &vals[i])) 
            return false;
    
  }
  return true;
}

bool sensor_callback(pb_istream_t *stream, const pb_field_t *sensors, void **arg)
{
    Sensor sens;
    
    if (!pb_decode(stream, Sensor_fields, &sens))
        return false;
    
    Serial.print(sens.id); 
    Serial.print(" : "); 
    Serial.println(sens.value);
    
    return true;
}

void setup() {
  Serial.begin(115200);

  uint8_t buffer[128];
  size_t message_length;
  bool status;

  vals[0].id = 23;
  vals[0].value = 4567;
  vals[1].id = 89;
  vals[1].value = 1234;

GardenSensors message = GardenSensors_init_default;
  
  /* Create a stream that will write to our buffer. */
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

   

   message.sensors.funcs.encode = &sensorValues_callback;

  /* Now we are ready to encode the message! */
        status = pb_encode(&stream, GardenSensors_fields, &message);
        message_length = stream.bytes_written;

        /* Then just check for any errors.. */
        if (!status)
        {
            Serial.println(PB_GET_ERROR(&stream));
        }

   /* Allocate space for the decoded message. */
        GardenSensors gs = GardenSensors_init_zero;

   /* Create a stream that reads from the buffer. */
        pb_istream_t i_stream = pb_istream_from_buffer(buffer, message_length);
        
        /* Now we are ready to decode the message. */
        status = pb_decode(&i_stream, GardenSensors_fields, &gs);

   /* Check for errors... */
        if (!status)
        {
            Serial.println(PB_GET_ERROR(&stream));
        }
        else {

          Serial.print("GardenSensors: ");
          /* Print the data contained in the message. */
          gs.sensors.funcs.decode = &sensor_callback;
        
        
        }

}

void loop() {


}
