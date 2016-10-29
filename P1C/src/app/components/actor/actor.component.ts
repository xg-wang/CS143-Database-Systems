import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';
import { Actor } from '../../models/actor';
import { ActorMovieRelation } from '../../models/actor-movie-relation';

@Component({
  selector: 'app-actor',
  templateUrl: './actor.component.html',
  styleUrls: ['./actor.component.css']
})
export class ActorComponent implements OnInit {
  actorInfo: Actor;
  amRelations: ActorMovieRelation[];

  constructor(
    private _bruimdbService: BruimdbServiceService,
    private _route: ActivatedRoute
  ) { }

  ngOnInit() {
    this._route.params
      .map(params => params['id'])
      .subscribe(id => {
        this._bruimdbService.searchActor(id)
          .subscribe(res => {
            this.actorInfo = res.actorInfo;
            this.amRelations = res.amRelations;
          });
      });
  }

}
