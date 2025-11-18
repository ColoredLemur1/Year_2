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
import java.util.concurrent.ThreadLocalRandom; // Used for generating random data
import java.time.LocalDateTime;

/**
 * Azure Function for COMP3211 Coursework 2, Task 1.
 * This function simulates sensor data and writes it to an Azure SQL Database.
 * Triggered by a timer every 10 seconds.
 */
public class Function {

    
    private static final int SENSOR_COUNT = 20; //
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

    
    @FunctionName("DataFunction")
    public void run(
            @TimerTrigger(name = "timerInfo", schedule = "*/10 * * * * *") String timerInfo,
            final ExecutionContext context) {
        
        context.getLogger().info("Timer trigger 'DataFunction' executed at: " + LocalDateTime.now());

        // --- Main Logic: Try-with-resources for DB connection and statement ---
        try (Connection connection = getConnection(context)) {
            context.getLogger().info("Database connection established.");

            // Get the next available sensor_id to avoid primary key violations
            int nextSensorId = resolveNextSensorId(connection, context);
            context.getLogger().info("Starting sensor_id from: " + nextSensorId);

            // Use a PreparedStatement for batch inserts, which is much faster
            try (PreparedStatement insertStatement = connection.prepareStatement(INSERT_SQL)) {
                
                // Loop 20 times, once for each sensor as per the spec
                // Start from the next available sensor_id
                for (int i = 0; i < SENSOR_COUNT; i++) {
                    int sensorId = nextSensorId + i;
                    
                    // 1. Generate random data based on the ranges in the PDF
                    int temp = getRandomNumber(TEMP_MIN, TEMP_MAX);
                    int wind = getRandomNumber(WIND_MIN, WIND_MAX);
                    int humidity = getRandomNumber(HUMIDITY_MIN, HUMIDITY_MAX);
                    int co2 = getRandomNumber(CO2_MIN, CO2_MAX);

                    // 2. Set parameters for the SQL statement
                    insertStatement.setInt(1, sensorId);
                    insertStatement.setInt(2, temp);
                    insertStatement.setInt(3, wind);
                    insertStatement.setInt(4, humidity);
                    insertStatement.setInt(5, co2);

                    // 3. Add this statement to the batch
                    insertStatement.addBatch();
                }

                // 4. Execute the entire batch of 20 inserts at once
                int[] updateCounts = insertStatement.executeBatch();
                context.getLogger().info("Successfully inserted " + updateCounts.length + " sensor readings.");
            }

        } catch (Exception e) {
            // Catch generic Exception to handle SQL or IO errors
            context.getLogger().severe("Error inserting sensor data: " + e.getMessage());
            // Print the stack trace to the Function App's logs for debugging
            e.printStackTrace();
        }
    }

    /**
     * Helper method to generate a random number between min and max (inclusive).
     * @param min Minimum value (inclusive)
     * @param max Maximum value (inclusive)
     * @return Random integer between min and max
     */
    private int getRandomNumber(int min, int max) {
        return ThreadLocalRandom.current().nextInt(min, max + 1);
    }

    /**
     * Helper method to resolve the next available sensor_id from the database.
     * This prevents primary key violations when running the function multiple times.
     * @param connection Database connection
     * @param context Execution context for logging
     * @return Next available sensor_id (MAX(sensor_id) + 1, or 1 if table is empty)
     */
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

    /**
     * Helper method to get a database connection.
     * This uses the environment variables set in the Function App's configuration.
     */
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