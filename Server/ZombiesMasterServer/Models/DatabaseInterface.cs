using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Web;

namespace ZombiesMasterServer.Models
{
    public class DatabaseInterface
    {
        private MySqlConnection SqlConnection;
        private string ConnectionString;

        public DatabaseInterface()
        {
            ConnectionString = ConfigurationManager.ConnectionStrings["WorldAtWarConnectionString"].ConnectionString;
            SqlConnection = new MySqlConnection(ConnectionString);
        }

        public int PostData(ServerData data)
        {
            try
            {
                SqlConnection.Open();

                MySqlCommand command = new MySqlCommand("AddServerEntry", SqlConnection);
                command.CommandType = System.Data.CommandType.StoredProcedure;

                // generate a random ID for the server
                Random random = new Random();
                int serverId = random.Next(1, int.MaxValue);

                command.Parameters.AddWithValue("_ServerID", serverId);
                command.Parameters.AddWithValue("_IPAddress", data.IPAddress);
                command.Parameters.AddWithValue("_ServerName", data.ServerName);
                command.Parameters.AddWithValue("_MapName", data.MapName);
                command.Parameters.AddWithValue("_CurrentPlayers", data.CurrentPlayers);
                command.Parameters.AddWithValue("_MaxPlayers", data.MaxPlayers);

                command.ExecuteNonQuery();

                SqlConnection.Close();

                return serverId;
            }
            catch (Exception ex)
            {
                return -1;
            }
        }

        public void DeleteData(int serverId)
        {
            try
            {
                SqlConnection.Open();

                MySqlCommand command = new MySqlCommand("DeleteServerEntry", SqlConnection);
                command.CommandType = System.Data.CommandType.StoredProcedure;

                command.Parameters.AddWithValue("_ServerID", serverId);

                command.ExecuteNonQuery();

                SqlConnection.Close();
            }
            catch (Exception ex)
            {

            }
        }
    }
}