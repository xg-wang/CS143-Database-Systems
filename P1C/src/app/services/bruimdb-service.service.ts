import { Injectable } from '@angular/core';
import { Http, Headers, RequestOptions } from '@angular/http';
import 'rxjs/add/operator/map';

const apiUrl = 'http://localhost:1438/~cs143/server/';
const searchApiUrl = apiUrl + 'search.php?';
const addRelationUrl = apiUrl + 'addRelation.php?';
const addEntityUrl = apiUrl + 'addEntity.php?';

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
    let searchUrl = searchApiUrl + 'entity=actors&name=' + str;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }
  searchMovieByName(str: string) {
    let searchUrl = searchApiUrl + 'entity=movies&name=' + str;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }

  searchActorById(id: number) {
    let searchUrl = searchApiUrl + 'entity=actors&id=' + id;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }
  searchMovieById(id: number) {
    let searchUrl = searchApiUrl + 'entity=movies&id=' + id;
    return this._http.get(searchUrl)
      .map(res => res.json());
  }

  addEntity(data: any) {
    let headers = new Headers({ 'Content-Type': 'application/json' });
    let options = new RequestOptions({ headers: headers });
    let url = addEntityUrl;
    console.log(url);
    console.log(data);
    return this._http.post(url, data, options);
    // TODO: retrurn msg!
  }
}
