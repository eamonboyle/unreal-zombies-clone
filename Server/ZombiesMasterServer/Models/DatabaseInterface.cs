using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Web;
using ZombiesMasterServer.Helpers;

namespace ZombiesMasterServer.Models
{
    public class DatabaseInterface
    {
        private MySqlConnection SqlConnection;
        private string ConnectionString;

        public DatabaseInterface()
        {
            //ConnectionString = ConfigurationManager.ConnectionStrings["WorldAtWarConnectionString"].ConnectionString;
            //SqlConnection = new MySqlConnection(ConnectionString);
        }

        public string GetUserIPAddress()
        {
            string IP = System.Web.HttpContext.Current.Request.UserHostAddress;

            if (IP == "::1")
            {
                IP = "127.0.0.1";
            }

            return IP;
        }

        public int PostData(ServerData data)
        {
            try
            {
                // delete any lobbies with this IP address before adding a new one
                SqlCommand deleteCommand = new SqlCommand("[dbo].[DeleteServerEntry]");
                deleteCommand.CommandType = CommandType.StoredProcedure;
                deleteCommand.Parameters.AddWithValue("IPAddress", GetUserIPAddress());

                Database.BaseNonQuery(deleteCommand);

                // generate a random ID for the server
                Random random = new Random();
                int serverId = random.Next(1, int.MaxValue);

                SqlCommand command = new SqlCommand("AddServerEntry");
                command.CommandType = CommandType.StoredProcedure;
                command.Parameters.AddWithValue("ServerID", serverId);
                command.Parameters.AddWithValue("IPAddress", GetUserIPAddress());
                command.Parameters.AddWithValue("ServerName", data.ServerName);
                command.Parameters.AddWithValue("MapName", data.MapName);
                command.Parameters.AddWithValue("CurrentPlayers", data.CurrentPlayers);
                command.Parameters.AddWithValue("MaxPlayers", data.MaxPlayers);

                bool entered = Database.BaseNonQuery(command);

                return serverId;
            }
            catch (Exception ex)
            {
                return -1;
            }
        }

        public void DeleteData()
        {
            try
            {
                SqlCommand command = new SqlCommand("DeleteServerEntry");
                command.CommandType = CommandType.StoredProcedure;
                command.Parameters.AddWithValue("IPAddress", GetUserIPAddress());

                Database.BaseNonQuery(command);
            }
            catch (Exception ex)
            {

            }
        }

        public bool UpdateServerEntry(ServerData data)
        {
            try
            {
                SqlCommand command = new SqlCommand("UpdateServerEntry");
                command.CommandType = CommandType.StoredProcedure;
                command.Parameters.AddWithValue("IPAddress", GetUserIPAddress());
                command.Parameters.AddWithValue("ServerName", data.ServerName);
                command.Parameters.AddWithValue("MapName", data.MapName);
                command.Parameters.AddWithValue("CurrentPlayers", data.CurrentPlayers);
                command.Parameters.AddWithValue("MaxPlayers", data.MaxPlayers);

                return Database.BaseNonQuery(command);
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        public DataTable GetAllServers()
        {
            try
            {
                SqlCommand command = new SqlCommand("GetAllServerEntries");
                command.CommandType = CommandType.StoredProcedure;

                DataTable servers = Database.BaseDataQuery(command);
                servers.TableName = "Servers";

                return servers;
            }
            catch (Exception ex)
            {
                return null;
            }
        }
    }
}