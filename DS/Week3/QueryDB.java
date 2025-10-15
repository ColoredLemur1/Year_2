import java.io.*;
import java.sql.*;
import java.util.*;


/**
 * Example of how to query a database using JDBC.
 *
 * <p>The program demonstrates</p>
 * <ul>
 *   <li>Use of properties to hold JDBC driver and database details</li>
 *   <li>Use of the SQL command SELECT</li>
 *   <li>Processing of ResultSet objects</li>
 * </ul>
 *
 * @author Karim Djemame and Nick Efford
 * @version 2.2 [2024-10-02]
 */

public class QueryDB {

/**
   * Establishes a connection to the database.
   *
   * The details of which driver to use, which database to
   * access and the username and password to use are being
   * hard-coded. 
   * Refer to the connection string, JDBC SQL authentication
   * on Azure 
   *
   * @return Connection object representing the connection
   * @throws IOException if properties file cannot be accessed
   * @throws SQLException if connection fails
   */

  public static Connection getConnection() throws IOException, SQLException
  {
    
    // Obtain access parameters and use them to create connection
    //
    //
    String dbServer = System.getenv("DB_SERVER");
    String dbName = System.getenv("DB_NAME");
    String dbUser = System.getenv("DB_USER");
    String dbPassword = System.getenv("DB_PASSWORD");

    if (dbServer == null || dbName == null || dbUser == null || dbPassword == null) {
        throw new SQLException("Database credentials are not set in environment variables. Please set DB_SERVER, DB_NAME, DB_USER, and DB_PASSWORD.");
    }

    // Build the connection URL from the environment variables
    String connectionUrl = String.format(
        "jdbc:sqlserver://%s.database.windows.net:1433;database=%s;user=%s@%s;password=%s;encrypt=true;trustServerCertificate=false;hostNameInCertificate=*.database.windows.net;loginTimeout=30;",
        dbServer, dbName, dbUser, dbServer, dbPassword);

    // Establish the connection
    Connection connection = DriverManager.getConnection(connectionUrl);

    return connection;
  }

  

  /**
   * Queries the database to find student names.
   * @param forename forename to search for in database
   * @param database connection to database
   * @throws SQLException if query fails
   */

  public static void findNames(String forename, Connection database)
   throws SQLException
  {
    Statement statement = database.createStatement();
    ResultSet results = statement.executeQuery(
     "SELECT * FROM students WHERE forename = '" + forename + "'");
    while (results.next()) {
      String surname = results.getString("surname");
      System.out.println(forename + " " + surname);
    }
    statement.close();
  }

  public static void listAllNames(Connection database)
   throws SQLException
  {
    Statement statement = database.createStatement();
    ResultSet results = statement.executeQuery(
     "SELECT forename, surname FROM students");
    while (results.next()) {
      String forename = results.getString("forename");
      String surname = results.getString("surname");
      System.out.println(forename + " " + surname);
    }
    statement.close();
  }

  public static void countNames(Connection database)
   throws SQLException
  {
    Statement statement = database.createStatement();
    ResultSet results = statement.executeQuery(
     "SELECT COUNT(*) AS total FROM students");
    if (results.next()) {
      int total = results.getInt("total");
      System.out.println("Total students: " + total);
    }
    statement.close();
  }


  /**
   * Main program.
   */

  public static void main(String[] argv)
  {
    if (argv.length == 0) {
      System.err.println("usage: java QueryDB <forename>");
      System.exit(1);
    }

    Connection connection = null;
 
    try {
      connection = getConnection();
      listAllNames(connection);
      countNames(connection);
    }
    catch (Exception error) {
      error.printStackTrace();
    }
    finally {

      // This will always execute, even if an exception has
      // been thrown elsewhere in the code - so this is
      // the ideal place to close the connection to the DB...

      if (connection != null) {
        try {
          connection.close();
        }
        catch (Exception error) {}
      }
    }
  }


}
