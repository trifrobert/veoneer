#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

void runPythonScript(const char* scriptPath, char result[]) {
    char command[256];
    sprintf(command, "python3 %s", scriptPath);
  
    // Open a pipe to execute the Python script
    FILE* pipe = popen(command, "r");
    if (!pipe) {
        printf("Error opening pipe to Python script.\n");
    }

    char buffer[255];

    // Read the output of the Python script
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        printf("Python script returned: %s\n", buffer);
        strcpy(result, buffer);
    }
    pclose(pipe);
}

char query[256];

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int verificare_numar(char* str, MYSQL *con){
  
  if(!isalpha(str[0])){
   return 0; 
  }
  
  snprintf(query, sizeof(query), "SELECT NrMatricol FROM cars where NrMatricol='%s'", str);
  
  
  if (mysql_query(con, query)){
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL){
    finish_with_error(con);
  }
  
  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  
  while ((row = mysql_fetch_row(result)))
  {
      for(int i = 0; i < num_fields; i++)
      {
        return row[i] ? 1 : 0;
      }
      
  }
  
}

int main(int argc, char **argv)
{
  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }
  
  if (mysql_real_connect(con, "localhost", "root", "1234", "testare", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, "DROP TABLE IF EXISTS cars")) {
      finish_with_error(con);
  }

  if (mysql_query(con, "CREATE TABLE cars(NrMatricol VARCHAR(255) PRIMARY KEY)")) {
      finish_with_error(con);
  }

  //if (mysql_query(con, "INSERT INTO cars VALUES('B777LVF')")) {
   //   finish_with_error(con);
  //}

  if (mysql_query(con, "INSERT INTO cars VALUES('B123VUM')")) {
      finish_with_error(con);
  }

  if (mysql_query(con, "INSERT INTO cars VALUES('TM97WTX')")) {
      finish_with_error(con);
  }

  if (mysql_query(con, "INSERT INTO cars VALUES('IF098952')")) {
      finish_with_error(con);
  }

  if (mysql_query(con, "INSERT INTO cars VALUES('B301THK')")) {
      finish_with_error(con);
  }
  if (mysql_query(con, "INSERT INTO cars VALUES('AR99KTA')")) {
      finish_with_error(con);
  }
    if (mysql_query(con, "INSERT INTO cars VALUES('B58PKW')")) {
      finish_with_error(con);
  }
  
  int serial_port;
    
  if((serial_port = serialOpen("/dev/ttyUSB0", 9600)) < 0){
    fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
    return -1;
  }
    
  if(wiringPiSetup() == -1){
    fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
    return 1;
  }
    
  char dat;
  const char* scriptPath = "script.py";
  char buffer[256]; 
    
  while(1){
    if(serialDataAvail(serial_port)){
      dat = serialGetchar(serial_port);
      printf("Char before:%c\n", dat);
      if(dat == '1'){
        runPythonScript(scriptPath, buffer);
  
        int i = 0;
        while(isdigit(buffer[i]) || isalpha(buffer[i])){
          i++;
        }
        char to_check[i+1];
        strncpy(to_check, buffer, i);
        //printf("\n__%s__\n", to_check);
  
        if(verificare_numar(to_check, con) == 1){
          serialPuts(serial_port, "1");
          printf("%s\n", "Am gasit");
        }
        else{
          serialPuts(serial_port, "0");
          printf("%s\n", "Nu am gasit");
        }
      }
      fflush(stdout);
    }
  }
  
  mysql_close(con);
  exit(0);
}













