#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

char query[256];

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int verificare_numar(char* str, MYSQL *con){
  
  snprintf(query, sizeof(query), "SELECT NrMatricol FROM cars where NrMatricol='%s'\n", str);
  
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
        return row[i] ? 1 : 0 ;
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

  if (mysql_query(con, "INSERT INTO cars VALUES('B777LVF')")) {
      finish_with_error(con);
  }

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
  printf("%d", verificare_numar("AR01LMM", con));
  
  mysql_close(con);
  exit(0);
}














