package Year_2.DS.coursework2;

import com.microsoft.azure.functions.*;
import com.microsoft.azure.functions.annotation.*;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.concurrent.ThreadLocalRandom; 
import java.time.LocalDateTime;

/**
 * Azure Function for COMP3211 Coursework 2, Task 3.
 * This function simulates sensor data and writes it to the database every 10 seconds based on the enviroment variable
 * This function also inserts a log into the PerformanceMetrics table to track the start and end times of the batch.
 */
public class Function {

    private static final int TEMP_MIN = 5;
    private static final int TEMP_MAX = 18;
    private static final int WIND_MIN = 12;
    private static final int WIND_MAX = 24;
    private static final int HUMIDITY_MIN = 30;
    private static final int HUMIDITY_MAX = 60;
    private static final int CO2_MIN = 400;
    private static final int CO2_MAX = 1600;

    private static final String INSERT_SQL =
        "INSERT INTO sensors (sensor_id, temperature, windspeed, relative_humidity, CO2) VALUES (?, ?, ?, ?, ?)";
    
    private static final String LOG_START_SQL =
        "INSERT INTO PerformanceMetrics (sensor_count, start_time) VALUES (?, SYSUTCDATETIME())";

    
    @FunctionName("DataFunction")
    public void run(
            @TimerTrigger(name = "timerInfo", schedule = "*/10 * * * * *") String timerInfo,
            final ExecutionContext context) {
        
        context.getLogger().info("Timer trigger 'DataFunction' executed at: " + LocalDateTime.now());

        // Read the number of readings per sensor from the environment variable
        String sensorCountEnv = System.getenv("TEST_SENSOR_COUNT");
        int sensorCount = Integer.parseInt(sensorCountEnv);
        context.getLogger().info("Processing " + sensorCount + " sensors (from TEST_SENSOR_COUNT=" + sensorCountEnv + ")");

        // try to connect to the database and log the start of the sensor
        try (Connection connection = getConnection(context)) {
            context.getLogger().info("Database connection established.");

            // log the start of the sensor
            try (PreparedStatement logStartStatement = connection.prepareStatement(LOG_START_SQL)) {
                logStartStatement.setInt(1, sensorCount);
                logStartStatement.executeUpdate();
                context.getLogger().info("Logged batch start with sensor_count: " + sensorCount);
            }

            // get the next available sensor_id to avoid primary key violations
            int nextSensorId = resolveNextSensorId(connection, context);
            context.getLogger().info("Starting sensor_id from: " + nextSensorId);

            try (PreparedStatement insertStatement = connection.prepareStatement(INSERT_SQL)) {
                
                // Loop sensorCount times, once for each sensor reading
                for (int i = 0; i < sensorCount; i++) {
                    int sensorId = nextSensorId + i;
                    
                    // randomise the values for the sensor reading
                    int temp = getRandomNumber(TEMP_MIN, TEMP_MAX);
                    int wind = getRandomNumber(WIND_MIN, WIND_MAX);
                    int humidity = getRandomNumber(HUMIDITY_MIN, HUMIDITY_MAX);
                    int co2 = getRandomNumber(CO2_MIN, CO2_MAX);

                    // set the parameters for the SQL insert statement
                    insertStatement.setInt(1, sensorId);
                    insertStatement.setInt(2, temp);
                    insertStatement.setInt(3, wind);
                    insertStatement.setInt(4, humidity);
                    insertStatement.setInt(5, co2);

                    insertStatement.addBatch();
                }

                // execute all the sensor reading inserts
                int[] updateCounts = insertStatement.executeBatch();
                context.getLogger().info("Successfully inserted " + updateCounts.length + " sensor readings.");
            }

        } catch (Exception e) {
            // catch any errors
            context.getLogger().severe("Error inserting sensor data: " + e.getMessage());
            e.printStackTrace();
        }
    }

    // generate a random number between min and max (helper function)
    private int getRandomNumber(int min, int max) {
        return ThreadLocalRandom.current().nextInt(min, max + 1);
    }

    // get the next available sensor_id to avoid primary key violations
    private int resolveNextSensorId(Connection connection, ExecutionContext context) throws SQLException {
        final String query = "SELECT ISNULL(MAX(sensor_id), 0) FROM sensors";
        try (Statement statement = connection.createStatement();
             ResultSet results = statement.executeQuery(query)) {
            if (results.next()) {
                int maxId = results.getInt(1);
                return maxId + 1;
            }
        }
        return 1;
    }

    // get a database connection (helper function)
    private Connection getConnection(ExecutionContext context) throws SQLException, IOException {
        String dbServer = System.getenv("DB_SERVER");
        String dbName = System.getenv("DB_NAME");
        String dbUser = System.getenv("DB_USER");
        String dbPassword = System.getenv("DB_PASSWORD");

        if (dbServer == null || dbName == null || dbUser == null || dbPassword == null) {
            throw new SQLException("Database credentials are not in environment variables. Please set DB_SERVER, DB_NAME, DB_USER, and DB_PASSWORD.");
        }

        String connectionUrl = String.format(
            "jdbc:sqlserver://%s.database.windows.net:1433;database=%s;user=%s@%s;password=%s;encrypt=true;trustServerCertificate=false;hostNameInCertificate=*.database.windows.net;loginTimeout=30;",
            dbServer, dbName, dbUser, dbServer, dbPassword
        );

        Connection connection = DriverManager.getConnection(connectionUrl);
        return connection;
    }
}