/* tslint:disable:no-unused-variable */
import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { By } from '@angular/platform-browser';
import { DebugElement } from '@angular/core';

import { AddDmRelationComponent } from './add-dm-relation.component';

describe('AddDmRelationComponent', () => {
  let component: AddDmRelationComponent;
  let fixture: ComponentFixture<AddDmRelationComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AddDmRelationComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AddDmRelationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
