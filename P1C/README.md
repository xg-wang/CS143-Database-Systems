# P1C
## API
### end points
1. `/api/v1/actors/`
  - GET
  - POST 
2. `/api/v1/directors`
  - POST
3. `/api/v1/movies`
  - GET
  - POST
4. `/api/v1/movies/<id>`
  - PUT
  
### search url
- universal search
  `http://localhost:1438/~cs143/server/api/v1/actors/search?name=Tom Hanks`
- search by id
  - `http://localhost:1438/~cs143/server/api/v1/actors/:id`
  - `http://localhost:1438/~cs143/server/api/v1/movies/:id`

### response date type
```json
Response Data type: json
{
   ‘success’ : false,
   ‘message’ : “All actors”,
   ‘data’ : [
	  {
		  ‘Id’: “”
		  ‘name’: “”
		  “id” : “12312”,
    },
   ]
}
```

### TODOs!!!
1. When search actor/movie by name, only return:
  - name/title
  - dob/year
2. When search by id,
  - For actor/director, return actor info, movies & role
  - For movie, return movie info, actors in movie, user reviews

Details see [demo](http://oak.cs.ucla.edu/classes/cs143/project/demo/p1c/Show_M.php?identifier=205).

