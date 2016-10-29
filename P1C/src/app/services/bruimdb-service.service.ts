import { Injectable } from '@angular/core';
import { Http } from '@angular/http';
import 'rxjs/add/operator/map';

const apiUrl = 'http://localhost:1438/~cs143/server/api/v1/';

@Injectable()
export class BruimdbServiceService {

  constructor(private _http: Http) { }

  search(str: string, actorOption = true, movieOption = true) {
    return {
      actors: actorOption ? this.searchActor(str) : null,
      movies: movieOption ? this.searchMovie(str) : null
    };
  }
  searchActor(str: string) {
    let searchUrl = apiUrl + 'actors/search?name=' + str;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }
  searchMovie(str: string) {
    let searchUrl = apiUrl + 'movies/search?name=' + str;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }

}
