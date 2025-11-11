package Year_2.DS.coursework2;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.LinkedList;
import java.util.Queue;
import java.util.StringTokenizer;

public class sensorSimulator {

    private static final String CSV_FILE = "sensor_readings.csv";
    private static final String INSERT_SQL =
        "INSERT INTO sensors (sensor_id, temperature, windspeed, relative_humidity, CO2) VALUES (?, ?, ?, ?, ?)";

    private static Queue<String> sensorLines = new LinkedList<>();

    public static Connection getConnection() throws IOException, SQLException {
        String dbServer = System.getenv("DB_SERVER");
        String dbName = System.getenv("DB_NAME");
        String dbUser = System.getenv("DB_USER");
        String dbPassword = System.getenv("DB_PASSWORD");

        if (dbServer == null || dbName == null || dbUser == null || dbPassword == null) {
            throw new SQLException("Database credentials are not in enviroment variables");
        }

        String connectionUrl = String.format(
            "jdbc:sqlserver://%s.database.windows.net:1433;database=%s;user=%s@%s;password=%s;encrypt=true;trustServerCertificate=false;hostNameInCertificate=*.database.windows.net;loginTimeout=30;",
            dbServer, dbName, dbUser, dbServer, dbPassword
        );

        Connection connection = DriverManager.getConnection(connectionUrl);
        return connection;
    }

    public static int insertSensorData(Connection database, int sensorId) throws SQLException {
        if (sensorLines.isEmpty()) {
            loadSensorLines();
        }

        String line = sensorLines.poll();
        if (line == null) {
            throw new SQLException("No sensor data available in CSV file.");
        }

        try (PreparedStatement insertStatement = database.prepareStatement(INSERT_SQL)) {
            StringTokenizer parser = new StringTokenizer(line, ",");
            if (parser.countTokens() != 4) {
                throw new SQLException("Invalid data format in CSV: " + line);
            }

            insertStatement.setInt(1, sensorId);
            insertStatement.setInt(2, Integer.parseInt(parser.nextToken().trim()));
            insertStatement.setInt(3, Integer.parseInt(parser.nextToken().trim()));
            insertStatement.setInt(4, Integer.parseInt(parser.nextToken().trim()));
            insertStatement.setInt(5, Integer.parseInt(parser.nextToken().trim()));
            insertStatement.executeUpdate();
        }
        sensorLines.add(line);
        return sensorId + 1;
    }

    public static void simulatedSensorts(Connection database, int numberOfSensors) throws SQLException {
        long startTime = System.currentTimeMillis();

        int nextSensorId = resolveNextSensorId(database);
        for (int i = 0; i < numberOfSensors; i++) {
            nextSensorId = insertSensorData(database, nextSensorId);
        }

        long duration = System.currentTimeMillis() - startTime;
        System.out.println("Inserted sensor data " + numberOfSensors + " times in " + duration + " ms.");
    }

    private static int resolveNextSensorId(Connection database) throws SQLException {
        final String query = "SELECT ISNULL(MAX(sensor_id), 0) FROM sensors";
        try (Statement statement = database.createStatement();
             ResultSet results = statement.executeQuery(query)) {
            if (results.next()) {
                return results.getInt(1) + 1;
            }
        }
        return 1;
    }

    private static void loadSensorLines() throws SQLException {
        try (BufferedReader reader = new BufferedReader(new FileReader(CSV_FILE))) {
            String line;
            while ((line = reader.readLine()) != null) {
                if (!line.trim().isEmpty()) {
                    sensorLines.add(line);
                }
            }
        } catch (IOException error) {
            throw new SQLException("Failed to read CSV file: " + CSV_FILE, error);
        }

        if (sensorLines.isEmpty()) {
            throw new SQLException("CSV file does not contain any sensor data.");
        }
    }

    public static void main(String[] args) {
        int numberOfSensors = 1;
        if (args.length > 0) {
            try {
                numberOfSensors = Integer.parseInt(args[0]);
            } catch (NumberFormatException error) {
                System.err.println("Invalid number of sensors provided, defaulting to 1.");
            }
        }

        Connection database = null;
        try {
            database = getConnection();
            System.out.println("Connected to database.");
            simulatedSensorts(database, numberOfSensors);
        } catch (Exception error) {
            error.printStackTrace();
        } finally {
            if (database != null) {
                try {
                    database.close();
                } catch (SQLException error) {
                    System.err.println("Failed to close database connection: " + error.getMessage());
                }
            }
        }
    }
}
