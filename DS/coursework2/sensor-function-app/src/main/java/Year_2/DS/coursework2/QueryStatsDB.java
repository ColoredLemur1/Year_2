package Year_2.DS.coursework2;

import com.microsoft.azure.functions.*;
import com.microsoft.azure.functions.annotation.*;

import java.io.IOException;
import java.sql.*;
import java.time.LocalDateTime;
import java.util.Optional;

/**
 * Azure Function for querying sensor statistics from the database every time the sensors table is updated.
 * This function is called by the SQL trigger which uses an app key to be configured in the azure app
 * This function also updates the end_time in the PerformanceMetrics table to track the end time of the batch.
 */
public class QueryStatsDB {

    private static final String LOG_END_SQL =
        "UPDATE PerformanceMetrics SET end_time = SYSUTCDATETIME() WHERE id = (SELECT TOP 1 id FROM PerformanceMetrics WHERE end_time IS NULL ORDER BY id DESC)";

    // query the sensor statistics on http trigger
    @FunctionName("QueryStatsDB")
    public void run(
            @HttpTrigger(
                name = "req",
                methods = {HttpMethod.POST, HttpMethod.GET},
                authLevel = AuthorizationLevel.FUNCTION)
            HttpRequestMessage<Optional<String>> request,
            final ExecutionContext context) {
        
        context.getLogger().info("QueryStatsDB triggered by SQL database change at: " + LocalDateTime.now());
        context.getLogger().info("Database modification detected - querying sensor statistics.");

        // Read the number of readings per sensor from the environment variable (this is needed for the grouping of the readings per sensor due to table configuration)
        String sensorCountEnv = System.getenv("TEST_SENSOR_COUNT");
        int sensorCount = Integer.parseInt(sensorCountEnv);
        context.getLogger().info("Using sensor_count: " + sensorCount + " (from TEST_SENSOR_COUNT=" + sensorCountEnv + ")");
        
        // try to connect to the database and query the sensor statistics
        try (Connection connection = getConnection(context)) {
            context.getLogger().info("Database connection established.");

            StringBuilder result = new StringBuilder();
            result.append("Sensor Statistics Query Results\n");
            result.append("================================\n");
            result.append("Timestamp: ").append(LocalDateTime.now()).append("\n");
            result.append("Sensor Count (Batch Size): ").append(sensorCount).append("\n\n");

            // execute all the queries to get the complete statistics
            result.append(getTempStats(connection, context, sensorCount));
            result.append("\n");
            result.append(getWindStats(connection, context, sensorCount));
            result.append("\n");
            result.append(getHumidityStats(connection, context, sensorCount));
            result.append("\n");
            result.append(getCO2Stats(connection, context, sensorCount));

            // log the end time of the sensor
            try (PreparedStatement logEndStatement = connection.prepareStatement(LOG_END_SQL)) {
                int updateCount = logEndStatement.executeUpdate();
                context.getLogger().info("Logged batch end time. Updated rows: " + updateCount);
            }

            context.getLogger().info("Successfully retrieved all statistics.");
            context.getLogger().info(result.toString());

        } catch (Exception e) {
            context.getLogger().severe("Error querying database: " + e.getMessage());
            e.printStackTrace();
        }
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

    // get the temperature statistics
    private String getTempStats(Connection database, ExecutionContext context, int sensorCount) throws SQLException {
        StringBuilder result = new StringBuilder();
        result.append("Temperature Stats:\n");
        
        // order by the first column, which is sensor_number
        String query = String.format(
            "SELECT ((sensor_id - 1) / %d) + 1 AS sensor_number, " +
            "MIN(temperature) AS min_temp, MAX(temperature) AS max_temp, AVG(temperature) AS avg_temp " +
            "FROM sensors GROUP BY ((sensor_id - 1) / %d) + 1 ORDER BY 1",
            sensorCount, sensorCount);
        
        try (Statement statement = database.createStatement();
             ResultSet results = statement.executeQuery(query)) {
            
            while (results.next()) {
                int sensorNumber = results.getInt("sensor_number");
                int minTemp = results.getInt("min_temp");
                int maxTemp = results.getInt("max_temp");
                float avgTemp = results.getFloat("avg_temp");
                result.append(String.format("Sensor %-2d | Max=%-2d | Min=%-2d | Avg=%.2f%n", 
                    sensorNumber, maxTemp, minTemp, avgTemp));
            }
        }
        return result.toString();
    }

    // get the wind speed statistics
    private String getWindStats(Connection database, ExecutionContext context, int sensorCount) throws SQLException {
        StringBuilder result = new StringBuilder();
        result.append("Wind Speed Stats:\n");
        
        String query = String.format(
            "SELECT ((sensor_id - 1) / %d) + 1 AS sensor_number, " +
            "MIN(windspeed) AS min_wind, MAX(windspeed) AS max_wind, AVG(windspeed) AS avg_wind " +
            "FROM sensors GROUP BY ((sensor_id - 1) / %d) + 1 ORDER BY 1",
            sensorCount, sensorCount);
        
        try (Statement statement = database.createStatement();
             ResultSet results = statement.executeQuery(query)) {
            
            while (results.next()) {
                int sensorNumber = results.getInt("sensor_number");
                int minWind = results.getInt("min_wind");
                int maxWind = results.getInt("max_wind");
                float avgWind = results.getFloat("avg_wind");
                result.append(String.format("Sensor %-2d | Max=%-2d | Min=%-2d | Avg=%.2f%n", 
                    sensorNumber, maxWind, minWind, avgWind));
            }
        }
        return result.toString();
    }

    // get the humidity statistics
    private String getHumidityStats(Connection database, ExecutionContext context, int sensorCount) throws SQLException {
        StringBuilder result = new StringBuilder();
        result.append("Relative Humidity Stats:\n");
        
        String query = String.format(
            "SELECT ((sensor_id - 1) / %d) + 1 AS sensor_number, " +
            "MIN(relative_humidity) AS min_humidity, MAX(relative_humidity) AS max_humidity, AVG(relative_humidity) AS avg_humidity " +
            "FROM sensors GROUP BY ((sensor_id - 1) / %d) + 1 ORDER BY 1",
            sensorCount, sensorCount);
    
        try (Statement statement = database.createStatement();
             ResultSet results = statement.executeQuery(query)) {
            
            while (results.next()) {
                int sensorNumber = results.getInt("sensor_number");
                int minHumidity = results.getInt("min_humidity");
                int maxHumidity = results.getInt("max_humidity");
                float avgHumidity = results.getFloat("avg_humidity");
                result.append(String.format("Sensor %-2d | Max=%-2d | Min=%-2d | Avg=%.2f%n", 
                    sensorNumber, maxHumidity, minHumidity, avgHumidity));
            }
        }
        return result.toString();
    }

    // get the CO2 statistics
    private String getCO2Stats(Connection database, ExecutionContext context, int sensorCount) throws SQLException {
        StringBuilder result = new StringBuilder();
        result.append("CO2 Stats:\n");
        
        String query = String.format(
            "SELECT ((sensor_id - 1) / %d) + 1 AS sensor_number, " +
            "MIN(CO2) AS min_co2, MAX(CO2) AS max_co2, AVG(CO2) AS avg_co2 " +
            "FROM sensors GROUP BY ((sensor_id - 1) / %d) + 1 ORDER BY 1",
            sensorCount, sensorCount);
    
        try (Statement statement = database.createStatement();
             ResultSet results = statement.executeQuery(query)) {
            
            while (results.next()) {
                int sensorNumber = results.getInt("sensor_number");
                int minCO2 = results.getInt("min_co2");
                int maxCO2 = results.getInt("max_co2");
                float avgCO2 = results.getFloat("avg_co2");
                result.append(String.format("Sensor %-2d | Max=%-2d | Min=%-2d | Avg=%.2f%n", 
                    sensorNumber, maxCO2, minCO2, avgCO2));
            }
        }
        return result.toString();
    }
}
