import { Injectable } from '@angular/core';
import { Http } from '@angular/http';
import 'rxjs/add/operator/map';

const apiUrl = 'http://localhost/search.php?';
@Injectable()
export class BruimdbServiceService {
  constructor(private _http: Http) { }

  search(str: string, actorOption = true, movieOption = true) {
    return {
      actors: actorOption ? this.searchActorByName(str) : null,
      movies: movieOption ? this.searchMovieByName(str) : null
    };
  }
  searchActorByName(str: string) {
    let searchUrl = apiUrl + 'entity=actors&name=' + str;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }
  searchMovieByName(str: string) {
    let searchUrl = apiUrl + 'entity=movies&name=' + str;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }

  searchActorById(id: number) {
    let searchUrl = apiUrl + 'entity=actors&id=' + id;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }
  searchMovieById(id: number) {
    let searchUrl = apiUrl + 'entity=movies&id=' + id;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }

}
