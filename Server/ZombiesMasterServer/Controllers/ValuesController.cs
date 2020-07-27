using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using ZombiesMasterServer.Models;

namespace ZombiesMasterServer.Controllers
{
    public class ValuesController : ApiController
    {
        // GET api/values
        public IEnumerable<string> Get()
        {
            return new string[] { "value1", "value2" };
        }

        // GET api/values/5
        public ServerData Get(int id)
        {
            ServerData Data = new ServerData();
            Data.Test();

            return Data;
        }

        // POST api/values
        public void Post([FromBody] string value)
        {
        }

        // PUT api/values/5
        public void Put(int id, [FromBody] string value)
        {
        }

        // DELETE api/values/5
        public void Delete(int id)
        {
        }
    }
}
